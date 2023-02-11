#include "server.h"
#define MAX 2000
#define PORT 8000
#define EXIT ":exit"

void server()
{
	int sock, client_sock, server_size;
	struct sockaddr_in server, client;
	char buffer[MAX];

	// Create socket
	
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Unable to create socket...\n");
		exit(1);
	}
	puts("Socket created successfully...");


	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;

	// Bind
	if(bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("Bind failed...\n");
		close(sock);
		exit(1);
	}
	puts("Bind done...");

	// Listen
	if(listen(sock, 3) == 0)
		puts("Listenning...");
	else
		puts("Unable to listen to client...");

	// Accept incoming connections
	server_size = sizeof(struct sockaddr_in);
	while(1) {
		client_sock = accept(sock, (struct sockaddr *)&client, (socklen_t *)&server_size);
		if(client_sock < 0) {
			perror("Accept failed...");
			exit(1);
		}
		printf("Disconnected from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

		// Response to client
		if(fork() == 0) {
			close(sock);

			while(1) {
				recv(client_sock, buffer, MAX, 0);
				if(strcmp(buffer, EXIT) == 0 || buffer[0] == '\0') {
					printf("Disconnected from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
					fflush(stdin);
					close(client_sock);
					break;
				} else {
					printf("Client: %s\n", buffer);
					send(client_sock, buffer, MAX, 0);
					fflush(stdout);
					memset(buffer, '\0', MAX);
				}
			}
		}
	}

	// Close the sockets
	close(sock);
	close(client_sock);
}
