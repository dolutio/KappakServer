#include "chat.h"

void load_chats_data()
{
	char* json_s = read_file("chats.json");

	if (!json_s)
	{
		printf("File reading error");
		free(json_s); // created with malloc in 'read_file' function
		return;
	}

	cJSON* root = cJSON_Parse(json_s);
	cJSON* item = NULL;
	free(json_s);
	
	cJSON_ArrayForEach(item, root)
	{
		if (cJSON_IsObject(item))
		{
			Chat* chat = (Chat*)malloc(sizeof(Chat));

			List* members = create_list();
			List* messages = create_list();
			char* last_msg_id;

			cJSON* members_json = cJSON_GetObjectItem(item, "members");
			cJSON* messages_json = cJSON_GetObjectItem(item, "messages");
			cJSON* last_msg_id_json = cJSON_GetObjectItem(item, "last_msg_id");

			if (members_json && cJSON_IsArray(members_json))
			{
				int members_count = cJSON_GetArraySize(members_json);
				
				for (int i = 0; i < members_count; i++)
				{
					cJSON* member_json = cJSON_GetArrayItem(members_json, i);					
					list_append(members, member_json->valuestring);
				}
			}

			if (messages_json && cJSON_IsArray(messages_json))
			{
				int messages_count = cJSON_GetArraySize(messages_json);

				for (int i = 0; i < messages_count; i++)
				{
					cJSON* message_json = cJSON_GetArrayItem(messages_json, i);

					list_append(messages, message_json->valuestring);
				}
			}

			if (last_msg_id_json && cJSON_IsString(last_msg_id_json))
				last_msg_id = last_msg_id_json->valuestring;

			chat->chat_name = item->string;
			chat->last_msg_id = last_msg_id;
			chat->members = members;
			chat->messages = messages;

			printf("\nAdd chat %s from json\n", chat->chat_name);list_append(chats, chat);
		}
	}
}

void add_chat_in_base(char* json_s)
{
	cJSON* root = cJSON_Parse(json_s);

	if (!root)
	{
		printf("Parsing Error");

		exit(1);
	}

	List* members = create_list();

	cJSON* members_json_array = cJSON_GetObjectItem(root, "members");
	
	if (!cJSON_IsArray(members_json_array))
	{
		printf("Error: members_json_array object is not an Array");

		exit(1);
	}

	int members_json_array_len = cJSON_GetArraySize(members_json_array);

	for (int i = 0; i < members_json_array_len; i++)
	{
		list_append(members, cJSON_GetArrayItem(members_json_array, i)->valuestring);
	}

	List* messages = create_list();
	
	cJSON* messages_json_array = cJSON_GetObjectItem(root, "messages");

	if (!cJSON_IsArray(messages_json_array))
	{
		printf("Error: messages_json_array object is not an Array");

		exit(1);
	}

	int messages_json_array_len = cJSON_GetArraySize(messages_json_array);

	for (int i = 0; i < messages_json_array_len; i++)
	{
		list_append(messages, cJSON_GetArrayItem(messages_json_array, i)->valuestring);
	}

	cJSON_Delete(root);

	Chat* chat = (Chat*)malloc(sizeof(Chat));
	chat->members = members;
	chat->messages = messages;
	chat->last_msg_id = "";

	list_append(chats, chat);
}

void print_chats()
{
	Node* chat_node = chats->head;

	while (chat_node)
	{	
		Chat* chat = (Chat*)chat_node->data;
		printf("Members: ");
		printf("%s\n", "a");
		printf("\nMESSages: ");
		printf("%d", chat->messages == NULL);
		printf("test");
		printf("\nLast Message: %s\n", chat->last_msg_id);

		chat_node = chat_node->next;
	}
	
	printf("\nEnd\n");
}

void save_chats_data()
{
	cJSON* root = cJSON_CreateObject();
	Node* chat_node = chats->head;

	while (chat_node)
	{
		Chat* chat = (Chat*)chat_node->data;
		printf("\nCmode:%d\n", chat==NULL);
		
		cJSON* members_json_array = cJSON_CreateArray();	
		printf("\nCmode:%d\n", members_json_array==NULL);
		
		printf("\nTest Debug: chat-%p\nmembers-%p\n", (void*)chat, (void*)chat->members);
		if (chat && chat->members) 
			printf("\nLen: %s\n", chat->chat_name);
		Node* member_node = chat->members->head;
		while (member_node)
		{
			cJSON_AddItemToArray(members_json_array, cJSON_CreateString((char*)member_node->data));
printf("\nCHAT\n");
			member_node = member_node->next;
		}

		cJSON* messages_json_array = cJSON_CreateArray();

		Node* message_node = chat->messages->head;

		while (message_node)
		{
			cJSON_AddItemToArray(messages_json_array, cJSON_CreateString((char*)message_node->data));

			message_node = message_node->next;
		}

		
		cJSON* chat_json = cJSON_CreateObject();
		cJSON_AddItemToObject(chat_json, "members", members_json_array);
		cJSON_AddItemToObject(chat_json, "messages", messages_json_array);
		cJSON_AddItemToObject(chat_json, "last_msg_id", cJSON_CreateString(chat->last_msg_id));
		cJSON_AddItemToObject(root, chat->chat_name, chat_json);
		

		chat_node = chat_node->next;
	}
	
	char* json_s = cJSON_Print(root);
	printf("\nJSon: %s\n", json_s);
	cJSON_Delete(root);

	save_file("chats.json", json_s);
}

