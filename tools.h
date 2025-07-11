#ifndef TOOLS_H
	#define TOOLS_H

	#include <stdio.h>
	#include <string.h>
	#include "cJSON/cJSON.h"
	#include "linked_list.h"
	#include "dictionary.h"

	List* split(const char* text, const char* delim);
	void handle_json_msg(char* json_s);
	char* get_msg_chat_name(char* json_s);
	char* read_file(const char* filename);
	void save_file(const char* filename, const char* json_s);
	void save_datas();
#endif
