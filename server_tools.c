#include "server_tools.h"

Dictionary* accounts;
List* clients;
List* chats;

void handle_req(int cl_fd, const char* request)
{
	List* splited_req_list = split(request, " ");

	char* cmd = (char*)splited_req_list->head->data;printf("%s\n", cmd);

	if (memcmp(cmd, "signup", 6) == 0) // signup
	{
		list_print(splited_req_list);
		char* username = (char*)splited_req_list->head->next->data;
		char* pwd_hash = (char*)splited_req_list->head->next->next->data;
		signup(cl_fd, username, pwd_hash);
	}

	else if (memcmp(cmd, "login", 5) == 0)
	{
		char* username = (char*)splited_req_list->head->next->data;
		char* pwd_hash = (char*)splited_req_list->head->next->next->data;

		login(cl_fd, username, pwd_hash);
	}

	else if (memcmp(cmd, "message", 7) == 0)
	{
		char* message_json_s = (char*)malloc(strlen(request) - 1); // -2 for first 2 chars and +1 for '\0'
		memcpy(message_json_s, &request[7], strlen(request) - 2); // 2?8?
		printf("\nMessage: %s FD: %d\n", message_json_s, cl_fd);

		Node* chat_node = chats->head;

		while (chat_node)
		{
			Chat* chat = chat_node->data;
			char* chat_name = get_msg_chat_name(message_json_s);

			if (strcmp(chat->chat_name, chat_name) == 0)
			{
				printf("\nMessage append\n");
				list_append(chat->messages, message_json_s);
				break;
			}

			chat_node = chat_node->next;
		}
		printf("\nSending %s\n", message_json_s);

		send_message(cl_fd, message_json_s);
		// uint8_t type = 0x01;
		// 	size_t len = strlen(message_json_s);
		// 	uint32_t len_net = htonl((uint32_t)len);

		// 	send(cl_fd, &type, 1, 0);
		// 	send(cl_fd, &len_net, 4, 0);
		// 	send(cl_fd, message_json_s, len, 0);

	}

	else if (memcmp(cmd, "addc", 4) == 0) // add chat
	{
		char* chat_name = (char*)splited_req_list->head->next->data;
		printf("\nChatname: %s\n", chat_name);

		add_chat(cl_fd, chat_name);save_chats_data();
	}

	else if (memcmp(cmd, "addu", 4) == 0) // add user in chat 
	{
		char* chat_name = (char*)splited_req_list->head->next->data;
		char* username = (char*)splited_req_list->head->next->next->data;

		add_user_in_chat(chat_name, username);
	}

	else if (memcmp(cmd, "slr", 3) == 0)
	{
	
	}

	else if (memcmp(cmd, "shut", 4) == 0)
	{
		save_datas();
		exit(0);
	}

	list_free(splited_req_list, NULL);
}

void send_resp(int cl_fd, int response)
{
	uint8_t _resptonse_type = 0x02;
	size_t _response_len = 4; // for uint32_t codes
	uint32_t _response_len_net = htonl((uint32_t)_response_len);
	uint32_t _response = htonl(response);

	send(cl_fd, &_resptonse_type, 1, 0);
	send(cl_fd, &_response_len_net, 4, 0);
	send(cl_fd, &_response, _response_len, 0);
}


void signup(int cl_fd, char* username, char* pwd_hash)
{
	Node* client_node = clients->head;

	while (client_node)
	{
		Client* client = (Client*)client_node;

		if (strcmp(client->username, username) == 0) // Account found
		{
			send_resp(cl_fd, ACCOUNT_ALREADY_EXISTS);

			return;
		}

		client_node = client_node->next;
	}

	Client* client = (Client*)malloc(sizeof(Client));
	client->username = username;
	client->pwd_hash = pwd_hash;
	client->cl_fd = cl_fd;

	list_append(clients, client);

	send_resp(cl_fd, ACCEPT);printf("\nACcetpted\n");

}

void login(int cl_fd, char* username, char* pwd_hash)
{
	char* store_pwd_hash = get_pwd_hash_by_username(username);

	if (!store_pwd_hash)
	{
		send_resp(cl_fd, ACCOUNT_NOT_FOUND);printf("\nAccNotFound %s\n", username);print_clients();
		return;
	}
	printf("\nPwd_hash %s\n", pwd_hash);
	printf("\n%d\n", strcmp(pwd_hash, store_pwd_hash));
	if (strcmp(pwd_hash, store_pwd_hash) != 0)
	{
		send_resp(cl_fd, INCORRECT_PASSWORD);printf("IncorrectPasw\n%s\n%s\n\n\n\n\n\n\n", pwd_hash, store_pwd_hash);
		return;
	}

	send_resp(cl_fd, ACCEPT);

	Node* client_node = clients->head;

	while (client_node)
	{
		Client* client = (Client*)client_node->data;

		if (strcmp(client->username, username) == 0)
		{
			client->cl_fd = cl_fd;printf("\nLoged In! %d\n", cl_fd);

			return;
		};printf("\nCurr UsName: %s\n", client->username);

		client_node = client_node->next;
	};printf("\nUsername Not Found: %s\n\n\n\n", username);
}

void send_message(int cl_fd, char* message_json_s)
{
	Node* client_node = clients->head;printf("\nsend159\n");

	while (client_node)
	{
		Client* client = (Client*)client_node->data;
		printf("\nsend_164\n");
		
		if (client->cl_fd != cl_fd)
		{
			printf("send_167");
			uint8_t type = 0x01;
			size_t len = strlen(message_json_s);
			uint32_t len_net = htonl((uint32_t)len);

			send(client->cl_fd, &type, 1, 0);
			send(client->cl_fd, &len_net, 4, 0);
			send(client->cl_fd, message_json_s, len, 0);printf("\n%d %d sended\n", client->cl_fd, cl_fd);
		}

		client_node = client_node->next;
	}
}

void add_chat(int cl_fd, const char* chat_name)
{
	Node* chat_node = chats->head;

	while (chat_node)
	{
		Chat* chat = (Chat*)chat_node->data;

		if (strcmp(chat->chat_name, chat_name) == 0)
		{
			send_resp(cl_fd, CHAT_ALREADY_EXISTS);printf("Chat already exists");

			return;
		}

		chat_node = chat_node->next;
	}

	Chat* chat = (Chat*)malloc(sizeof(Chat));
	chat->chat_name = chat_name;
	chat->members = create_list();
	chat->messages = create_list();
	chat->last_msg_id = "";

	list_append(chats, chat);

	send_resp(cl_fd, ACCEPT);
	save_chats_data();
}

void add_user_in_chat(char* chat_name, char* username)
{
	Node* chat_node = chats->head;

	while (chat_node)
	{
		Chat* chat = (Chat*)chat_node->data;

		if (strcmp(chat->chat_name, chat_name) == 0)
		{
			list_append(chat->members, username);

			return;
		}

		chat_node = chat_node->next;
	}

	printf("Chat with name %s not found", chat_name);
}

void set_last_recv_msgs(int cl_fd, char* last_recv_msgs)
{
	//for chat in Client.chats last_recv_msgs = chat_name:last_recv_msg
}

void update_chats()
{
	Node* chat_node = chats->head;
	
	while (chat_node)
	{
		Chat* chat = chat_node->data;
		List* chat_members = chat->members;
		Node* client_node = chat_members->head;
		while(client_node)
		{
			Client* client = client_node->data;
	//		client->last_recv_msgs;
			client_node = client_node->next;
		}

		chat_node = chat_node->next;
	}
}
