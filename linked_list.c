#include "linked_list.h"

Node* create_node(void* data)
{
	Node* node = (Node*)malloc(sizeof(Node));

	if (!node)
	{
		perror("Error with mem alloc in Node.");
		exit(1);
	}

	node->data = data;
	node->next = NULL;

	return node;
}

List* create_list()
{
	List* list = (List*)malloc(sizeof(List));

	if (!list)
	{
		perror("Error with mem alloc in List");
		exit(1);
	}

	list->head = NULL;
	list->tail = NULL;

	return list;
}

void list_append(List* list, void* data)
{
	Node* node = create_node(data);
	list->len++;

	if (!list->head)
	{
		list->head = node;
	}
	
	else
	{
		Node* temp = node;

		list->tail->next = node;
	}
	
	list->tail = node;
}

void list_print(List* list)
{
	printf("List printing func\n");
	Node* temp = list->head;

	if (!temp) printf("Head is NULL");

	while (temp)
	{
		printf("List: %s\n", (char*)temp->data);
		temp = temp->next;
	}
}

bool list_contains(List* list, void* data, size_t data_size)
{
	Node* temp = list->head;

	while (temp)
	{
		if(memcmp(temp->data, data, data_size) == 0)
			return true;

		temp = temp->next;
	}

	return false;
}

void list_free(List* list, void (*data_free)(void*))
{
	Node* current = list->head;

	while (current)
	{
		Node* next = current->next;

		if (data_free)
		{
			data_free(current->data);
		}

		free(current);
		current = next;
	}

	free(list);
}
