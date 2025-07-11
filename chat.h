#ifndef CHAT_H
	#define CHAT_H

	#include "linked_list.h"
	#include "tools.h"
	#include "client.h"

	typedef struct Chat
	{
		const char* chat_name;
		char* last_msg_id;
		List* members; // List[Clients]
		List* messages;
	} Chat;

	extern List* chats;
	void load_chats_data();
	void print_chats();
	void save_chats_data();
	void chat_free();
	void chats_free();
#endif
