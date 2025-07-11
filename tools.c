#include "tools.h"

List* split(const char* text, const char* delim)
{
	List* text_list = create_list();
	if (!text_list) printf("LIst is null");
	
	size_t text_len = strlen(text);
	size_t delim_len = strlen(delim);

	size_t start = 0;
	for (int i = 0; i <= text_len - delim_len; i++)
	{
		if (memcmp(&text[i], delim, delim_len) == 0)
		{
			size_t len = i - start;

			char* part = (char*)malloc(len + 1);
			memcpy(part, &text[start], len);
			part[len] = '\0';
			list_append(text_list, part);
			start = i + delim_len;
			i += delim_len - 1;
		}
	}

	if (start <= text_len)
	{
		size_t len = text_len - start;

		char* part = (char*)malloc(len + 1);
		memcpy(part, &text[start], len);
		part[len] = '\0';

		list_append(text_list, part);
	}

	return text_list;
}

void parse_object(cJSON* obj)
{
	if (!obj)
	{
		printf("JSON parsing error\n");
		exit(1);
	}

	cJSON* item = NULL;

	cJSON_ArrayForEach(item, obj)
	{
		const char* key = item->string;

		if (cJSON_IsString(item))
		{
			printf("%s: %s\n", key, item->valuestring);
		}

		else if (cJSON_IsNumber(item))
		{
			printf("%s: %f\n", key, item->valuedouble);
		}

		else if (cJSON_IsObject(item))
		{
			printf("{\n");

			parse_object(item);

			printf("\n}");
		}
	}

	
}

void handle_json_msg(char* json_s)
{

}

char* read_file(const char* filename)
{
	FILE* file = fopen(filename, "rb");

	if (!file) return NULL;
	
	fseek(file, 0, SEEK_END);

	long int len = ftell(file);

	rewind(file);

	char* data = (char*)malloc(len + 1);

	if (!data)
	{
		fclose(file);
		return NULL;
	}

	fread(data, 1, len, file);
	data[len] = '\0';
	fclose(file);

	return data;
}

void save_file(const char* filename, const char* json_s)
{
	FILE* file = fopen(filename, "w");
	if (!file) return;

	fputs(json_s, file);
	fclose(file);
}

char* get_msg_chat_name(char* json_s)
{
	cJSON* root = cJSON_Parse(json_s);
	cJSON* chat_json = root->child;

	char* chat_name = chat_json->string;
	
	printf("Chat Name: %s\n", chat_name);

//	parse_object(root);

	cJSON_Delete(root);

	return chat_name;
}

void handle_chat(char* chat_s)
{
	cJSON* root = cJSON_Parse(chat_s);
	
	root;
	
}

void save_datas()
{
	
}
