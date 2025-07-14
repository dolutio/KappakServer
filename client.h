#ifndef CLIENT_H
	#define CLIENT_H

	#include <string.h>
	#include "linked_list.h"
	#include "dictionary.h"
	#include "tools.h"

	typedef struct Client
	{
		char* username;
		char* pwd_hash;
		int cl_fd;

		Dictionary* last_recv_msg;
	} Client;

	extern List* clients;
	void load_clients_data();
	char* get_pwd_hash_by_username(const char* username);
	void print_clients();
	void save_clients_data();
	void clients_free();
#endif
