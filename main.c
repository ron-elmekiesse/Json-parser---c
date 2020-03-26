#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

//help functions
#include "utilities.h"

//function to parse the json text
#include "parser_func.h"

#define NUM_INPUT 2
#define EXIT "exit"


int main(int argc, char* argv[]) // array of strings...
{
	FILE* fp = NULL;
	int file_length = 0;
	char* file_name = NULL;
	char* heap_file = NULL;
	char* ptr = NULL;
	char* token = NULL;
	struct Tav* head = NULL;
	struct Key* key_head = NULL;
	struct Key* key_temp = NULL;
	struct Value* value_head = NULL;
	struct Value* value_temp = NULL;
	int i = 0 , j = 0, keys_given = 0, value_founded = 0;
	char command[NAME_SIZE];
	char temp_command[NAME_SIZE];

	if (argc != NUM_INPUT)
	{
		//getting the ./<out name> and the json text
		printf("Usage: %s <file_name>\n", argv[0]); // The first argument is the name of the program
		return -1;
	}

	file_name = argv[1];
	file_length = get_file_length(file_name);

	//initializing place in the heap
	heap_file = (char*) malloc(file_length * sizeof(char));

	if (!heap_file)
	{
		printf("Problem in malloc allocating! \n");
		return -1;
	}

	fp = fopen(file_name, "r");
	
	if (read_file_to_heap(fp, file_length, heap_file))
	{
		printf("Error in reading the file content to the heap! \n");
		return -1;
	}

	fclose(fp);



	//the head of the linked list of the keys
	key_head = make_keys_list(heap_file, file_length);

	//the head of the linked list of the values
	value_head = make_values_list(heap_file, file_length);

	key_temp = key_head;
	
	value_temp = value_head;
	
	system("clear");

	while (1)
	{
		reset_char_array(command, NAME_SIZE);
		printf("\n---------------------------\n");
		printf("\n ~ Please enter the path of keys to see the value: \n");
		printf("\n ~ Example: key,key...  \n");
		printf("\n ~ To exit enter 'exit' \n");
		printf("\n ~ Enter: ");
		fgets(command, NAME_SIZE, stdin);

		ptr = strchr(command, '\n');
		if (ptr)
			*ptr = '\0';

		if (strcmp(command, EXIT) == 0)
		{
			printf("\n ~ Thanks for using our Json parser! (: \n");
			break;
		}

		strncpy(temp_command, command, NAME_SIZE); //another copy of the command from stdin

		token = strtok(command, ",");

		while (token != NULL)
		{

			if (strcmp(token, key_temp->name) == 0)
			{
				keys_given++;
				token = strtok(NULL, ",");

				if (token == NULL)
				{
					if (valid_path(heap_file, file_length, temp_command, keys_given)) //check for valid path --------------------
					{	
						switch (value_temp->type)
						{
							case STRING:
								printf("\nThe value is:    %s \n", value_temp->string_value);
								break;
							case INTEGER:
								printf("\nThe value is:    %d \n", value_temp->int_value);
								break;
							case DOUBLE:
								printf("\nThe value is:    %.2f \n", value_temp->double_value);
								break;
							default:
								printf("\nProblem in the switch! \n");
						}
						value_founded = 1;
					}
					else
					{
						printf("\n ~ ----> Please enter the full path!!!  <----\n");
					}
				}

			}

			else
			{
				key_temp = key_temp->next;
				value_temp = value_temp->next;
			}
		
			if (!key_temp && !value_founded) //if the keys entered are not valid
			{
				printf("\n ~ ----> The keys entered are not valid! <---- \n");
				break;
			}

		}
		
		value_founded = 0;

		key_temp = key_head;
	
		value_temp = value_head;

		keys_given = 0;
	}


	return 100;

}






//another small Json example:
//{"test" : "tootim", "fruits" : {"apple": "green","banana" :"yellow", "total": 5 , "number" : 9.9 , "another" : {"good" : {"pop" : [1,2,3,4,5]}}} , "king" : [1,2,3,"pop"] }