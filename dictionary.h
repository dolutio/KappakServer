#ifndef DICTIONARY_H
	#define DICTIONARY_H

	#include <stdio.h>
	#include <stdlib.h>
	#include <stdbool.h>
	#include <string.h>
	#include "linked_list.h"
	

	typedef struct KeyValuePair
	{
		void* key;
		void* value;

		size_t key_size;
		size_t value_size;
	} KeyValuePair;

	typedef struct Dictionary
	{
		List* keyvalue_pair_list;
		unsigned int size;
	} Dictionary;

	KeyValuePair* create_keyvalue_pair(void* key, void* value, size_t key_size, size_t value_size);
	Dictionary* create_dict();
	void dict_add_keyvalue_pair(Dictionary* dict, void* key, void* value, size_t key_size, size_t value_size);
	void* dict_get_value_by_key(Dictionary* dict, void* key);
	void dict_free(Dictionary* dict);

#endif
