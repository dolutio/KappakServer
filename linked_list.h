#ifndef LINKED_LIST_H
	#define LINKED_LIST_H

	#include <stdio.h>
	#include <stdlib.h>
	#include <stdbool.h>
	#include <string.h>
	
	typedef struct Node
	{
		void* data;
		struct Node* next;
	} Node;

	typedef struct List
	{
		unsigned int len;
		Node* head;
		Node* tail;
	} List;

	Node* create_node(void* data);
	List* create_list();
	void list_append(List* list, void* data);
	void list_print(List* list);
	bool list_contains(List* list, void* data, size_t data_size);
	void list_free(List* list, void (data_free)(void*));
#endif
