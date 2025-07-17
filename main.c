#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "linked_list.h"
#include "dictionary.h"
#include "tools.h"
#include "server_tools.h"

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_sigint(int sig);
void* handle_client(void* arg); // arg in pthread is void*

int main()
{
	signal(SIGINT, handle_sigint);

	printf("STart\n");
	accounts = create_dict();
	chats = create_list();
	clients = create_list();
	load_chats_data();
	load_clients_data();
	print_chats();
	print_clients();
	printf("\nAksljfio\n");

	int server_fd;printf("\n24\n");
	struct sockaddr_in server_addr;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd < 0)
	{
		perror("Socket Error");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		close(server_fd);
		exit(1);
	}

	if (listen(server_fd, 5) < 0)
	{
		perror("Listen Error");
		close(server_fd);
		exit(1);
	}

	while (true)
	{
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int* ptr_client_fd = (int*)malloc(sizeof(int));
		*ptr_client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

		if (*ptr_client_fd < 0)
		{
			perror("accept");
			close(server_fd);
			exit(1);
		}

		inet_ntoa(client_addr.sin_addr);

		pthread_t tid;
		pthread_create(&tid, NULL, handle_client, ptr_client_fd);
		pthread_detach(tid);
	}

	close(server_fd);
	save_chats_data();
	save_clients_data();

	return 0;
}

void handle_sigint(int sig)
{
	printf("Closing");
	// save_clients_data();
	save_chats_data();

	exit(0);
}

void* handle_client(void* arg)
{
	int cl_fd = *(int*)arg;
	free(arg);

	char buffer[BUFFER_SIZE];

	ssize_t bytes_received;

	while((bytes_received = recv(cl_fd, buffer, BUFFER_SIZE - 1, 0)) > 0)
	{
		buffer[bytes_received] = '\0';

		if (strlen(buffer) > 0)
		{
			handle_req(cl_fd, buffer);
		}
	}


	printf("Client disconnected");

	close(cl_fd);

	return NULL;
}