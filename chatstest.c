#include <stdio.h>
#include "cJSON/cJSON.h"

void parse_object(cJSON* obj)
{
	cJSON* item = NULL;
	cJSON_ArrayForEach(item, obj)
	{
		const char* key = item->string;

		if (cJSON_IsString(item)) printf("%s: %s\n", key, item->valuestring);
		else if (cJSON_IsNumber(item)) printf("%s: %f\n", key, item->valuedouble);
		else if (cJSON_IsObject(item))
		{
		 	printf("{\n");	
			parse_object(item);
			printf("\n}");
		}

		else printf("undef type");
	}
}

char* read_file(const char* filename)
{
	FILE* fp = fopen(filename, "r");

	if (fp == NULL)
	{
		printf("Error: Unable to open file.\n");
		exit(1);
	}

	char buffer[1024]
}

int main()
{
	const char* json_s = "";
	cJSON* json = cJSON_Parse(json_s);

	if (!json)
	{
		printf("Parsing Error");
		return 1;
	}

	parse_object(json);

	cJSON_Delete(json);

	return 0;
}
