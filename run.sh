#!/bin/bash

gcc main.c linked_list.h linked_list.c dictionary.h dictionary.c cJSON/cJSON.h cJSON/cJSON.c tools.h tools.c chat.h chat.c client.h client.c server_tools.h server_tools.c -o main && ./main
