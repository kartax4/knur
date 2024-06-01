#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

const char* STATIC_RESPONSE =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Content-Length: 155\r\n"
        "Server: KNURCAMP SERVER 2137\r\n"
        "Accept-Ranges: bytes\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html>\r\n"
        "  <head>\r\n"
        "    <title>An Example Page</title>\r\n"
        "  </head>\r\n"
        "  <body>\r\n"
        "    <p>Hello World, this is a very simple HTML document.</p>\r\n"
        "  </body>\r\n"
        "</html>";

const char* STATIC_RESPONSE_SUCHA_KREWETA =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Server: KNURCAMP SERVER 2137\r\n"
        "Accept-Ranges: bytes\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html>\r\n"
        "  <head>\r\n"
        "    <title>Suchy Kreweta</title>\r\n"
        "  </head>\r\n"
        "  <body>\r\n"
        "    <p>Hello this is suchy kreweta.</p>\r\n"
        "  </body>\r\n"
        "</html>";


#define INPUT_BUFFER_SIZE 4096

void findPath(const char* request, char* target) {
    while(*(request)++ != ' ') {}
    while(*request != ' ') {
        *(target)++ = *(request)++;
    }
    *target = 0x00;
}

const char* pseudoRouter(const char* requestedPath)
{
    if(strcmp("/suchyKreweta", requestedPath) == 0) {
        return STATIC_RESPONSE_SUCHA_KREWETA;
    }
    else if(strcmp("/vanish", requestedPath) == 0) {
        return NULL;
    }
    else {
        return STATIC_RESPONSE;
    }
}

int main(void) {
    // Tu trzymamy przychodzące dane od przeglądarki.
    char inputBuffer[INPUT_BUFFER_SIZE + 1];

    // Tu tworzymy socket servera IPv4.
    int server = socket(AF_INET, SOCK_STREAM, 0);

    // Tu obsługujemy błąd w tworzeniu socketa servera.
    if(server == -1)
    {
        printf("Nie udalo sie stworzyc socketa aha12\n");
        exit(-1);
    }

    // Tu przypisujemy adres IP i port do servera.
    struct sockaddr_in sai;
    sai.sin_addr.s_addr = inet_addr("127.0.0.1");   // IP na którym tworzymy server
    sai.sin_family = AF_INET;                       // IPv4
    sai.sin_port = htons(2137);                     // Port htons(port)
    memset(sai.sin_zero, 0, 8);                     // Padding - zawsze ustaw na 0.

    int ret = bind(server, (struct sockaddr*)&sai, sizeof(sai));

    // Tu obsługujemy bład przypisania IP i portu do servera.
    // WAŻNE żeby to obsłużyć bo ta operacja często może sie nie udać.
    if(ret != 0)
    {
        printf("Dupa nie dziala aha ok\n");
        exit(-1);
    }

    // Pozwalamy na połączenia przychodzące do naszego servera.
    // (Ustawiamy stan na LISTENING)
    // Te 1024 to backlog czyli kolejka oczekujących połączeń na zaakceptowanie.
    ret = listen(server, 1024);

    // Obsługa błędu przy ustawianiu stanu socketa.
    if(ret != 0)
    {
        printf("Dupa nie udalo sie nasluchiwac na komendzie w plocku\n");
        exit(-1);
    }

    // Obsługa połączeń przychodzących.
    // Pętla nieskończona po to żeby nasz server się nie wyłączył po obsłużeniu jednego klienta.
    while(1)
    {
        char pathBuffer[1024];
        char ipBuffer[16];

        struct sockaddr_in clientData;
        socklen_t size = sizeof(clientData);

        // Akceptujemy połączenie przychodzące.
        // Pod client mamy uchwyt do clienta.
        // Tego uchwytu używamy np. żeby coś do niego wysłać albo coś odebrać.
        int client = accept(server, (struct sockaddr*)&clientData, &size);

        // Obsługujemy błąd przy akceptacji.
        if(client == -1)
        {
            printf("Dupa blond w accept\n");
            continue;
        }

        printf("Accepted connection from %s:%d\n", inet_ntop(AF_INET, &clientData.sin_addr.s_addr, ipBuffer, 16), ntohs(clientData.sin_port));

        // Odbieramy request od przeglądarki.
        size_t received = recv(client, inputBuffer, INPUT_BUFFER_SIZE, 0);
        inputBuffer[received] = 0x00;

        findPath(inputBuffer, pathBuffer);
        printf("Requested path: %s\n", pathBuffer);

        // Wypisujemy request od przeglądarki.
        //printf("Received data from client:\n%s\n", inputBuffer);

        const char* response = pseudoRouter(pathBuffer);

        if(response == NULL)
        {
            // Zamykamy transmisje na sockecie clienta.
            shutdown(client, SHUT_RDWR);
            // Zamykamy socket clienta.
            close(client);
            break;
        }

        // Wysyłamy stringa do przeglądarki.
        size_t sent = send(client, response, strlen(response), 0);

        // Zamykamy transmisje na sockecie clienta.
        shutdown(client, SHUT_RDWR);
        // Zamykamy socket clienta.
        close(client);

        printf("Sent %zu bytes to client and closed connection.\n", sent);
    }

    close(server);

    return 0;
}
