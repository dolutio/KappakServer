#ifndef SERVER_TOOLS_H
	#define SERVER_TOOLS_H
	
	#include <stdio.h>
	#include <string.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include "linked_list.h"
	#include "dictionary.h"
	#include "tools.h"
	#include "chat.h"

	#define ACCEPT 0
	#define ACCOUNT_NOT_FOUND 1
	#define ACCOUNT_ALREADY_EXISTS 2
	#define INCORRECT_PASSWORD 3
	#define CHAT_ALREADY_EXISTS 4

	typedef struct Message
	{
		char* chat_name;
		char* msg_id;
	} Message;

	extern Dictionary* accounts;

	void handle_req(int cl_fd, const char* request);
	void send_resp(int cl_fd, int reply);
	void signup(int cl_fd, char* username, char* pwd_hash);
	void login(int cl_fd, char* username, char* pwd_hash);
	void add_chat(int cl_fd, const char* chat_name);
	void add_user_in_chat(char* chat_name, char* username);

#endif
