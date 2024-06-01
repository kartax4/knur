#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

const char *STATIC_RESPONSE =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Content-Length: 155\r\n"
        "Server: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\r\n"
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

#define INPUT_BUFFER_SIZE 4096

void findPath(const char *request, char *target) {
    while (*(request)++ != ' ') {}
    while (*request != ' ') {
        *(target)++ = *(request)++;
    }
    *target = 0x00;
}


int main(void) {
    char inputBuffer[INPUT_BUFFER_SIZE + 1];
    // Tu tworzymy socket servera IPv4.
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        printf("Error creating socket");
        exit(0);
    }

    // Tu przypisujemy adres IP i port do servera.
    struct sockaddr_in ipadr;
    ipadr.sin_addr.s_addr = inet_addr("0.0.0.0"); // IP na którym tworzymy server
    ipadr.sin_family = AF_INET; // IPv4
    ipadr.sin_port = htons(2137);   // Port htons(port)
    memset(ipadr.sin_zero, 0, 8);   // Padding - zawsze ustaw na 0.


    int response = bind(serverSocket, (struct sockaddr *) &ipadr, sizeof(ipadr));

    if (response != 0) {
        printf("Dupa nie dziala aha ok\n");
        exit(-1);
    }

    response = listen(serverSocket, 1024);
    if (response != 0) {
        printf("nie udalo sie");
        exit(-1);
    }

    while (1) {
        char pathBuffer[1024];
        struct sockaddr_in clientData;
        socklen_t size = sizeof(clientData);

        // Akceptujemy połączenie przychodzące.
        int clientSocket = accept(serverSocket, (struct sockaddr *) &ipadr, &size);


        // Obsługujemy błąd przy akceptacji.
        if (clientSocket == -1) {
            printf("Accept error");
            continue;
        }

        // Odbieramy request od przeglądarki.
        size_t received = recv(clientSocket, inputBuffer, INPUT_BUFFER_SIZE, 0);
        inputBuffer[received] = 0x00;

        findPath(inputBuffer, pathBuffer);
        printf("Requested path:\n%serverSocket\n", pathBuffer);

        // Wypisujemy request od przeglądarki.
        //printf("Received data from client:\n%serverSocket\n", inputBuffer);

        // Wysyłamy stringa do przeglądarki.
        size_t sent = send(clientSocket, STATIC_RESPONSE, strlen(STATIC_RESPONSE), 0);
        printf("Sent %zu bytes to client and closed connection.", sent);

        // Zamykamy transmisje na sockecie clienta.
        shutdown(clientSocket, SHUT_RDWR);
        // Zamykamy socket clienta.
        close(clientSocket);
    }
    return 0;
}