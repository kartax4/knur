#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/epoll.h>

const char *STATIC_RESPONSE =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html; charset=UTF-8\r\n"
		"Server: Kartax Server\r\n"
		"Accept-Ranges: bytes\r\n"
		"Connection: close\r\n"
		"\r\n"
		"<html>\r\n"
		"  <head>\r\n"
		"    <title>Root</title>\r\n"
		"  </head>\r\n"
		"  <body>\r\n"
		"    <p>Hello World, this is a very simple HTML document.</p>\r\n"
		"  </body>\r\n"
		"</html>";

const char *STATIC_RESPONSE_KARTAX =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html; charset=UTF-8\r\n"
		"Server: Kartax Server\r\n"
		"Accept-Ranges: bytes\r\n"
		"Connection: close\r\n"
		"\r\n"
		"<html>\r\n"
		"  <head>\r\n"
		"    <title>Kartax</title>\r\n"
		"  </head>\r\n"
		"  <body>\r\n"
		"    <p>Jesteś na innej stronie.</p>\r\n"
		"  </body>\r\n"
		"</html>";
const char *STATIC_RESPONSE_404 =
		"HTTP/1.1 404 Not Found\nConnection: close\n\n";

#define INPUT_BUFFER_SIZE   4096
#define MAX_CLIENTS 1024

void findPath(const char *request, char *target) {
	while (*(request)++ != ' ') {}
	while (*request != ' ') {
		*(target)++ = *(request)++;
	}
	*target = 0x00;
}

void handler_sig13() {
	printf("Broken pipe error\n");
}

const char *pseudoRouter(const char *requestedPath) {
	if (strcmp("/kartax", requestedPath) == 0) {
		return STATIC_RESPONSE_KARTAX;
	} else if (strcmp("/favicon.ico", requestedPath) == 0) {
		return STATIC_RESPONSE_404;
	} else if (strcmp("/vanish", requestedPath) == 0) {
		return NULL;
	} else {
		return STATIC_RESPONSE;
	}
}

int setnonblock(int socket) {
	int ret = fcntl(socket, F_SETFL, O_NONBLOCK);
	return ret;
}

void errorHandler(char *error) {
	printf("%s\n", error);
	exit(1);
}

int main(void) {
	signal(13, handler_sig13);
	//Dane od klienta
	char inputBuffer[INPUT_BUFFER_SIZE + 1];

	int ret;
	char pathBuffer[1024];
	char ipBuffer[16];
	struct sockaddr_in clientData;
	socklen_t size = sizeof(clientData);

	// Tu tworzymy socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		printf("Error creating socket");
		exit(-1);
	}
	//ustwaienie socketa na nieblokujacy
	ret = setnonblock(serverSocket);
	if (ret == -1) {
		printf("Fcntl error");
		exit(-1);
	}
	//struktra adresu ip
	struct sockaddr_in ipAddress;
	ipAddress.sin_addr.s_addr = inet_addr("0.0.0.0");        //IP na którym tworzymy server.
	ipAddress.sin_family = AF_INET;        // IPv4
	ipAddress.sin_port = htons(2137);        // Port htons(port)
	memset(ipAddress.sin_zero, 0, 8);        //Padding zawsze ustaw na 0.

	// Tu przypisujemy adres IP i port do servera.
	ret = bind(serverSocket, (struct sockaddr *) &ipAddress, sizeof(ipAddress));
	if (ret != 0) {
		errorHandler("Error when binding socket");
	}

	//ustawienie socketa na nasłuch
	ret = listen(serverSocket, 1024);
	if (ret != 0) {
		errorHandler("Error listening socket");
	}

	//tworzenie kolejki
	struct epoll_event event, events[MAX_CLIENTS];
	int epollQueue = epoll_create1(0);
	if (epollQueue == -1) {
		errorHandler("Error creating queue");
	}

	//Dodanie socketu servera
	event.events = EPOLLIN;
	event.data.fd = serverSocket;
	if (epoll_ctl(epollQueue, EPOLL_CTL_ADD, serverSocket, &event) == -1) {
		errorHandler("epoll_ctl: serverSocket");
	}

	int loop = 1;
	printf("Server started\n");
	while (loop) {
		int changes = epoll_wait(epollQueue, events, MAX_CLIENTS, -1);
		if (changes == -1) {
			errorHandler("epoll_wait error");
		}

		for (int i = 0; i < changes; ++i) {
			// Akceptujemy połączenie przychodzące, a jeśli .
			if (events[i].data.fd == serverSocket) {
				int clientSocket = accept(serverSocket, (struct sockaddr *) &ipAddress, &size);
				printf("Accepted connection from %s:%d\n",
					   inet_ntop(AF_INET, &clientData.sin_addr.s_addr, ipBuffer, 16), ntohs(clientData.sin_port));
				if (clientSocket == -1) {
					errorHandler("Accept Error");
				}
				//ustawniamy nonblock na socket clienta
				ret = setnonblock(clientSocket);
				if (ret == -1) {
					errorHandler("Fcntl error client");
				}

				//Dodanie socketu klienta do kolejki
				event.events = EPOLLIN | EPOLLET;
				event.data.fd = clientSocket;
				if (epoll_ctl(epollQueue, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
					errorHandler("epoll_ctl: conn_sock");
				}
			} else {
				//do_use_fd(events[i].data.fd);
				size_t received = recv(events[i].data.fd, inputBuffer, INPUT_BUFFER_SIZE, 0);
				inputBuffer[received] = 0x00;
				if (received == -1) {
					printf("Error occured on socket, closing %d.\n", i);
					shutdown(events[i].data.fd, SHUT_RDWR);
					close(events[i].data.fd);
				} else {
					findPath(inputBuffer, pathBuffer);
					printf("Requested path:\n%s by socket %d\n", pathBuffer, i);

					const char *response = pseudoRouter(pathBuffer);
					if (response == NULL) {
						shutdown(events[i].data.fd, SHUT_RDWR);
						close(events[i].data.fd);
						loop = 0;
						break;
					}
					// Wysyłamy stringa do przeglądarki.
					ssize_t sendReturn = send(events[i].data.fd, response, strlen(response), 0);
					shutdown(events[i].data.fd, SHUT_RDWR);
					close(events[i].data.fd);
					printf("Wysłano %zu bajtów i zamknięto.\n", sendReturn);
				}
			}
		}
	}
	close(epollQueue);
	shutdown(serverSocket, SHUT_RDWR);
	close(serverSocket);
	return 0;
}
