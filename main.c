#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

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

const char *STATIC_RESPONSE_KARTAX =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html; charset=UTF-8\r\n"
		"Content-Length: 155\r\n"
		"Server: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\r\n"
		"Accept-Ranges: bytes\r\n"
		"Connection: close\r\n"
		"\r\n"
		"<html>\r\n"
		"  <head>\r\n"
		"    <title>KARTAX</title>\r\n"
		"  </head>\r\n"
		"  <body>\r\n"
		"    <p>Jesteś na innej stronie</p>\r\n"
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

const char *Router(const char *requestedPath) {
	if (strcmp("/kartax", requestedPath) == 0) {
		return STATIC_RESPONSE_KARTAX;
	} else if (strcmp("/vanish", requestedPath) == 0) {
		return NULL;
	} else {
		return STATIC_RESPONSE;
	}
}

int main(void) {
	//miejsce na dane od klienta
	char inputBuffer[INPUT_BUFFER_SIZE + 1];

	// Tu tworzymy socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		printf("Error creating socket");
		exit(0);
	}

	int status = fcntl(serverSocket, F_SETFL, O_NONBLOCK);

	if (status == -1) {
		perror("calling fcntl");
		// handle the error.  By the way, I've never seen fcntl fail in this way
	}

	// Tu przypisujemy adres IP i port do servera.
	struct sockaddr_in ipadr;
	ipadr.sin_addr.s_addr = inet_addr("0.0.0.0"); //IP na którym tworzymy server.
	ipadr.sin_family = AF_INET; // IPv4
	ipadr.sin_port = htons(2137);   // Port htons(port)
	memset(ipadr.sin_zero, 0, 8);   //Padding zawsze ustaw na 0.


	int ret = bind(serverSocket, (struct sockaddr *) &ipadr, sizeof(ipadr));

	if (ret != 0) {
		printf("Error when binding socket\n");
		exit(-1);
	}
	//ustawienie socketa na nasłuch
	ret = listen(serverSocket, 1024);
	if (ret != 0) {
		printf("nie udalo sie");
		exit(-1);
	}

	while (1) {


		char pathBuffer[1024];
		char ipBuffer[16];
		struct sockaddr_in clientData;
		socklen_t size = sizeof(clientData);

		// Akceptujemy połączenie przychodzące.
		int clientSocket = accept(serverSocket, (struct sockaddr *) &ipadr, &size);


		// Obsługujemy błąd przy akceptacji.
		if (clientSocket == -1) {
			printf("Accept error");
			continue;
		}

		printf("Accepted connection from %s:%d\n", inet_ntop(AF_INET, &clientData.sin_addr.s_addr, ipBuffer, 16),
			   ntohs(clientData.sin_port));

		// Odbieramy request od przeglądarki.
		size_t received = recv(clientSocket, inputBuffer, INPUT_BUFFER_SIZE, 0);
		inputBuffer[received] = 0x00;

		findPath(inputBuffer, pathBuffer);
		printf("Requested path:\n%s\n", pathBuffer);

		//Wypisujemy request od przeglądarki.
		//printf("Received data from client:\n%serverSocket\n", inputBuffer);

		const char *response = Router(pathBuffer);

		if (response == NULL) {
			// Zamykamy transmisje na sockecie clienta.
			shutdown(clientSocket, SHUT_RDWR);
			// Zamykamy socket clienta.
			close(clientSocket);
			break;
		}

		// Wysyłamy stringa do przeglądarki.
		size_t sent = send(clientSocket, response, strlen(response), 0);
		printf("Sent %zu bytes to client and closed connection.", sent);

		// Zamykamy transmisje na sockecie clienta.
		shutdown(clientSocket, SHUT_RDWR);
		// Zamykamy socket clienta.
		close(clientSocket);

		printf("Sent %zu bytes to client and closed connection.\n", sent);
	}
	close(serverSocket);
	return 0;
}