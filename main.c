#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "linked_list.h"
#include "dictionary.h"
#include "tools.h"
#include "server_tools.h"

#define HOST ""
#define PORT 8080

void handle_sigint(int sig);

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
	printf("\nAksljfio\n");

	int server_fd, client_fd;printf("\n24\n");
	struct sockaddr_in server_addr, client_addr;
	char buffer[1024];
	socklen_t client_len = sizeof(client_addr);
	ssize_t bytes_received;printf("\n28\n");

	server_fd = socket(AF_INET, SOCK_STREAM, 0);printf("30");

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

	if (listen(server_fd, 1) < 0)
	{
		perror("Listen");
		close(server_fd);
		exit(1);
	}

	while (true)
	{
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

		if (client_fd < 0)
		{
			perror("accept");
			close(server_fd);
			exit(1);
		}

		inet_ntoa(client_addr.sin_addr);

		while ((bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0)
		{
			printf("73");
			buffer[bytes_received] = '\0';
			printf("\nClient: %s\n", buffer);

		//	dict_add_keyvalue_pair(accounts, "name", buffer, (size_t)bytes_received, (size_t)bytes_received);
		//	printf("Client\'s username is %s and password is %s", (char*)dict_get_value_by_key(accounts, "name"), buffer);
		//	printf("Is contains? %d\n ", list_contains(usernames, buffer, bytes_received));
			if (strlen(buffer) != 0)
				handle_req(client_fd, buffer);

		}

		close(client_fd);
	}
	close(server_fd);
	save_chats_data();
	save_clients_data();

	return 0;
}

void handle_sigint(int sig)
{
	printf("Closing");
	save_chats_data();

	exit(0);
}
