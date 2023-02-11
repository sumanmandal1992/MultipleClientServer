#include "client.h"
#define PORT 8000
#define MAX 2000
#define EXIT ":exit"

void client()
{
	int sock;
	struct sockaddr_in client;
	char buffer[MAX];

	// Creat socket
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Unable to create socket...\n");
		exit(1);
	}
	puts("Socket created successfully...");

	// Prepare the sockaddr_in structure
	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	client.sin_addr.s_addr = INADDR_ANY;

	// Connect to remote server
	if(connect(sock, (struct sockaddr *)&client, sizeof(client)) < 0) {
		perror("Unable to connect to server...\n");
		exit(1);
	}
	puts("Connected to server successfully...");

	// Build and send message to server
	while(1) {
		printf("Client: ");
		fflush(stdin);
		scanf("%s", buffer);
		send(sock, buffer, sizeof(buffer), 0);

		if(strcmp(buffer, EXIT) == 0 || buffer[0] == '\0') {
			close(sock);
			perror("Disconnected from server...\n");
			exit(1);
		}

		// Receive response from server
		if(recv(sock, buffer, sizeof(buffer), 0) < 0)
			perror("Error in receiving data...\n");
		else
			printf("Server: %s\n", buffer);
		fflush(stdout);
	}
}
