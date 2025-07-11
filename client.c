#include "client.h"

void load_clients_data()
{
	char* json_s = read_file("clients.json");

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
			Client* client = (Client*)malloc(sizeof(Client));
			client->username = item->string;
			
			cJSON* pwd_hash_json = cJSON_GetObjectItem(item, "pwd_hash");

			if (pwd_hash_json && cJSON_IsString(pwd_hash_json))
			{
				client->pwd_hash = pwd_hash_json->valuestring;
			}

			else
				printf("Error: clients password hash is not string");

			list_append(clients, client);
		}
	}
}

void add_client_in_base(char* json_s)
{
	cJSON* root = cJSON_Parse(json_s);

	if (!root)
	{
		printf("Parsing error");

		exit(1);
	}

	cJSON* user_json = root->child;

	if (!user_json || !cJSON_IsString(user_json))
	{
		printf("Error user object is not a String");

		exit(1);
	}


	char* username = user_json->string;
	char* pwd_hash = user_json->valuestring;

	Client* client = (Client*)malloc(sizeof(Client));

	client->username = username;
	client->pwd_hash = pwd_hash;
	
	list_append(clients, client);

	cJSON_Delete(root);
}

void save_clients_data()
{
	cJSON* root = cJSON_CreateObject();

	Node* client_node = clients->head;
	Node* item = NULL;

	while (client_node)
	{
		Client* client = client_node->data;

		cJSON_AddStringToObject(root, client->username, client->pwd_hash);
	}

	char* json_s = cJSON_Print(root);
	cJSON_Delete(root);

	save_file("clients.json", json_s);

	free(json_s);
}

char* get_pwd_hash_by_username(const char* username)
{
	Node* client_node = clients->head;

	while (client_node)
	{
		Client* client = (Client*)client_node->data;
		
		if (strcmp(client->username, username) == 0)
		{
			return client->pwd_hash;
		}

		client_node = client_node->next;
	}

	return NULL;
}

void clients_free()
{
	Node* client_node = clients->head;

	while (client_node)
	{
		Client* client = client_node->data;
		// free
		free(client_node->data);

		free(client_node);

		client_node = client_node->next;
	}

	list_free(clients, NULL);
}
