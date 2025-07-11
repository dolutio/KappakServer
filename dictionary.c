#include "dictionary.h"

KeyValuePair* create_keyvalue_pair(void* key, void* value, size_t key_size, size_t value_size)
{
	KeyValuePair* keyvalue_pair = (KeyValuePair*)malloc(sizeof(KeyValuePair));
	
	if (!keyvalue_pair)
	{
		perror("Error with mem alloc in KeyValuePair");
		exit(0);
	}

	keyvalue_pair->key = key;
	keyvalue_pair->value = value;

	return keyvalue_pair;
}

Dictionary* create_dict()
{
	Dictionary* dict = (Dictionary*)malloc(sizeof(Dictionary));

	if (!dict)
	{
		perror("Error with mem alloc in Dictionary");
		exit(1);
	}

	dict->keyvalue_pair_list = create_list();

	return dict;
}

void dict_add_keyvalue_pair(Dictionary* dict, void* key, void* value, size_t key_size, size_t value_size)
{
	list_append(dict->keyvalue_pair_list, create_keyvalue_pair(key, value, key_size, value_size));
	
}

void* dict_get_value_by_key(Dictionary* dict, void* key)
{
	Node* temp = dict->keyvalue_pair_list->head;
	
	while (temp)
	{
		KeyValuePair* keyvalue_pair = (KeyValuePair*)(temp->data);

		if (memcmp(keyvalue_pair->key, key, keyvalue_pair->key_size) == 0)
				return keyvalue_pair->value;
		
		temp = temp->next;
	}

	return NULL;
}

void dict_free(Dictionary* dict)
{
	Node* keyvalue_node = dict->keyvalue_pair_list->head;

	while (keyvalue_node)
	{
		free(keyvalue_node);

		keyvalue_node = keyvalue_node->next;
	}

	list_free(dict->keyvalue_pair_list, NULL);

	free(dict);
}
