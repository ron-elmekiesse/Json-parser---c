#include "utilities.h"
#include "parser_func.h"

struct Tav* make_list(char* heap_file, int length, char to_link)
{
	struct Tav* head = NULL;
	struct Tav* temp = NULL;
	int i = 0;

	for (i = 0; i < length; i++)
	{
		if (heap_file[i] == to_link)
		{
			if (!head)
			{
				head = (struct Tav*) malloc(sizeof(struct Tav));
				if (!head)
				{
					printf("problem allocating malloc! \n");
					return NULL;
				}
				head->next = NULL;
				head->index = i;
				temp = head;
			}

			else
			{
				temp->next = (struct Tav*) malloc(sizeof(struct Tav));
				if (!temp->next)
				{
					printf("problem allocating malloc! \n");
					return NULL;
				}
				temp->next->next = NULL;
				temp->next->index = i;
				temp = temp->next;
			}
		}
	}

	return head;
}

struct Key* make_keys_list(char* heap_file, int length)
{	//makes linked list of the keys in the Json file
	struct Tav* tav_head = make_list(heap_file, length, ':'); // going backwards
	struct Key* key_head = NULL;
	struct Key* temp = NULL;
	int counter = 1, i = 0, j = 0, temp_index = 0, start_index = 0, end_index = 0;

	tav_head = check_colon_list(heap_file, tav_head, length);

	while (tav_head != NULL)
	{
		temp_index = tav_head->index - 1;

		while (heap_file[temp_index] != '"')
		{//looking for the end of the key
			temp_index--;

			if (temp_index == -1 || heap_file[temp_index] == ',')
			{
				printf("Syntax error! < missing -> %c >\n", (char)34); //printing a single "
				return NULL;
			}
		}

		temp_index--; //starting from the last letter of the key
		end_index = temp_index;

		while (heap_file[temp_index] != '"')
		{ //going to the start of the key
			temp_index--;

			if (temp_index == -1 || heap_file[temp_index] == ',')
			{
				printf("Syntax error! < missing -> %c >\n", (char)34); //printing a single "
				return NULL;
			}
		}	

		temp_index++; // going to the first letter in the key
		start_index = temp_index;


		if (!key_head)
		{
			key_head = (struct Key*) malloc(sizeof(struct Key));
			key_head->next = NULL;
			key_head->placement = counter;
			key_head->name_length = string_value_length(heap_file, start_index); //with the null byte

			key_head->name = (char*) malloc(key_head->name_length);

			j = 0;
		
			for (i = start_index; i <= end_index; i++)
			{
				key_head->name[j] = heap_file[i];
				j++;
			}

			key_head->name[j] = '\0'; //adding the null byte in the end

			temp = key_head;
		}
			
		else
		{
			temp->next = (struct Key*) malloc(sizeof(struct Key));
			temp->next->next = NULL;
			temp->next->placement = counter;
			temp->next->name_length = string_value_length(heap_file, start_index); //with the null byte

			temp->next->name = (char*) malloc(temp->next->name_length);

			j = 0;
			
			for (i = start_index; i <= end_index; i++)
			{
				temp->next->name[j] = heap_file[i];
				j++;
			}

			temp->next->name[j] = '\0'; //adding the null byte in the end

			temp = temp->next;
		}
	
		counter++;

		tav_head = tav_head->next;
	}

	return key_head;
}

struct Value* make_values_list(char* heap_file, int length)
{	//making linked list of the value in the Json file
	struct Tav* tav_head = make_list(heap_file, length, ':');
	struct Value* value_head = NULL;
	struct Value* temp = NULL;
	int counter = 1, i = 0, first_head = 1, temp_index = 0;

	tav_head = check_colon_list(heap_file, tav_head, length);

	while (tav_head != NULL)
	{
		temp_index = tav_head->index + 1;
		i = 0;


			while (heap_file[temp_index] == ' ' || heap_file[temp_index] == '\n' || heap_file[temp_index] == '\t')
			{
				//keep going until you found the value
				temp_index++;
			}


			if (heap_file[temp_index] == '"')
			{
				//if the value is a string:
				temp_index++;
				
				if (!value_head)
				{

					temp = make_string_value(heap_file, temp_index, temp, length, counter);

					if (!temp)
					{
						//if there was a problem:
						printf("Problem in the string value! \n");

						return NULL;
					}

					value_head = temp;
				}
				else
				{
					temp->next = make_string_value(heap_file, temp_index, temp, length, counter);

					if (!temp)
					{
						//if there was a problem:
						printf("Problem in the string value! \n");

						return NULL;
					}

					first_head = 0;
				}
			}

			else if (heap_file[temp_index] == '{')
			{
				//if the value is an object:
				temp_index++;

				if (!value_head)
				{

					temp = make_object_value(heap_file, temp_index, temp, length, counter);

					if (!temp)
					{
						//if there was a problem:
						printf("Problem in the object value! \n");

						return NULL;
					}

					value_head = temp;
				}
				else
				{
					temp->next = make_object_value(heap_file, temp_index, temp, length, counter);

					if (!temp)
					{
						//if there was a problem:
						printf("Problem in the object value! \n");

						return NULL;
					}

					first_head = 0;
				}
				

			}

			else if (heap_file[temp_index] == '[')
			{
				//if the value is a list:
				temp_index++;

				
				if (!value_head)
				{

					temp = make_list_value(heap_file, temp_index, temp, length, counter);

					if (!temp)
					{
						//if there was a problem:
						printf("Problem in the list value! \n");

						return NULL;
					}

					value_head = temp;
				}
				else
				{
					temp->next = make_list_value(heap_file, temp_index, temp, length, counter);

					if (!temp)
					{
						//if there was a problem:
						printf("Problem in the list value! \n");

						return NULL;
					}

					first_head = 0;
				}

			}

			else
			{
				//if the value is an int or a double:

				if (!value_head)
				{
					temp = make_number_value(heap_file, temp_index, temp, length, counter);

					if (!temp)
					{
						//if there was a problem:
						printf("Problem in the number value! \n");

						return NULL;
					}

					value_head = temp;
				}
				else
				{
					temp->next = make_number_value(heap_file, temp_index, temp, length, counter);

					if (!temp)
					{
						//if there was a problem:
						printf("Problem in the number value! \n");

						return NULL;
					}

					first_head = 0;
				}

			}
		counter++;
	
		if (!first_head)
			temp = temp->next;


		tav_head = tav_head->next;

	}

	return value_head;
}

struct Value* make_string_value(char* heap_file, int start_index, struct Value* temp, int length, int counter)
{
	int i = 0, value_length = 0;

	value_length = string_value_length(heap_file, start_index);


	if (!temp)
	{
		temp = (struct Value*) malloc(sizeof(struct Value));
		temp->next = NULL;
		temp->placement = counter;
		temp->type = STRING;

		temp->string_length = value_length; //with the null byte
	
		temp->string_value = (char*) malloc(value_length);

		
		while (heap_file[start_index] != '"')
		{
			temp->string_value[i] = heap_file[start_index];
			start_index++;
			i++;
			if (start_index == length || heap_file[start_index] == ',')
			{
				printf("Syntax error! < missing -> %c >\n", (char)34); //printing a single "
				return NULL;
			}
		}

		temp->string_value[i] = '\0'; //adding the null byte

		return temp;
	}
	//else is not needed because of the return

	temp->next = (struct Value*) malloc(sizeof(struct Value));
	temp->next->next = NULL;
	temp->next->placement = counter;
	temp->next->type = STRING;

	temp->next->string_length = value_length; //with the null byte
	
	temp->next->string_value = (char*) malloc(value_length);

		
	while (heap_file[start_index] != '"')
	{
		temp->next->string_value[i] = heap_file[start_index];
		start_index++;
		i++;
		if (start_index == length || heap_file[start_index] == ',')
		{
			printf("Syntax error! < missing -> %c >\n", (char)34); //printing a single "
			return NULL;
		}
	}

	temp->next->string_value[i] = '\0'; //adding the null byte

	return temp->next;
	
}

struct Value* make_object_value(char* heap_file, int start_index, struct Value* temp, int length, int counter)
{
	int i = 0, put_space = 0, geresh_count = 0, another_object = 1, value_length = 0;

	value_length = object_value_length(heap_file, start_index);

	if (!temp)
	{
		temp = (struct Value*) malloc(sizeof(struct Value));
		temp->next = NULL;
		temp->placement = counter;
		temp->type = STRING;

		temp->string_length = value_length; //with the null byte
	
		temp->string_value = (char*) malloc(value_length);

		//making the output look better
		temp->string_value[i] = '\n';
		i++;
		temp->string_value[i] = '\t';
		i++;
		temp->string_value[i] = '{';
		i++;

		while (another_object > 0)
		{
			if (heap_file[start_index] != '"' && heap_file[start_index] != ' ' && heap_file[start_index] != '\n' && heap_file[start_index] != '\t')
			{
				temp->string_value[i] = heap_file[start_index];
				i++;
				put_space = 1;
			}
					
			else if (heap_file[start_index] != '"' && put_space)
			{
				//making the output look better

				temp->string_value[i] = '\n';
				i++;
				temp->string_value[i] = '\t';
				i++;
				put_space = 0;
			}


			if (heap_file[start_index] == '"')
				geresh_count++;

			if (heap_file[start_index] == '{' && geresh_count % 2 == 0)
				another_object++;

			if (heap_file[start_index] == '}' && geresh_count % 2 == 0)
				another_object--;


			start_index++;

			if (start_index == length)
			{
				printf("Syntax error! < missing -> ' } ' >\n");
				return NULL;
			}
		}

		temp->string_value[i] = '\0'; //adding the null byte

		return temp;
	}
	//else is not needed because of the return


	temp->next = (struct Value*) malloc(sizeof(struct Value));
	temp->next->next = NULL;
	temp->next->placement = counter;
	temp->next->type = STRING;

	temp->next->string_length = value_length; //with the null byte
	
	temp->next->string_value = (char*) malloc(value_length);

	//making the output look better
	temp->next->string_value[i] = '\n';
	i++;
	temp->next->string_value[i] = '\t';
	i++;
	temp->next->string_value[i] = '{';
	i++;

	while (another_object > 0)
	{
		if (heap_file[start_index] != '"' && heap_file[start_index] != ' ' && heap_file[start_index] != '\n' && heap_file[start_index] != '\t')
		{
			temp->next->string_value[i] = heap_file[start_index];
			i++;
			put_space = 1;
		}
					
		else if (heap_file[start_index] != '"' && put_space)
		{
			//making the output look better

			temp->next->string_value[i] = '\n';
			i++;
			temp->next->string_value[i] = '\t';
			i++;
			put_space = 0;
		}


		if (heap_file[start_index] == '"')
			geresh_count++;

		if (heap_file[start_index] == '{' && geresh_count % 2 == 0)
			another_object++;

		if (heap_file[start_index] == '}' && geresh_count % 2 == 0)
			another_object--;


		start_index++;

		if (start_index == length)
		{
			printf("Syntax error! < missing -> ' } ' >\n");
			return NULL;
		}
	}

	temp->next->string_value[i] = '\0'; //adding the null byte

	return temp->next;
	
}

struct Value* make_list_value(char* heap_file, int start_index, struct Value* temp, int length, int counter)
{
	int i = 0, geresh_count = 0, another_list = 1, put_space = 0, value_length = 0;
	
	value_length = object_value_length(heap_file, start_index);

	if (!temp)
	{
		temp = (struct Value*) malloc(sizeof(struct Value));
		temp->next = NULL;
		temp->placement = counter;
		temp->type = STRING;

		temp->string_length = value_length; //with the null byte
	
		temp->string_value = (char*) malloc(value_length);

		//making the output look better
		temp->string_value[i] = '\n';
		i++;
		temp->string_value[i] = '\t';
		i++;
		temp->string_value[i] = '[';
		i++;

		while (another_list > 0)
		{
			if (heap_file[start_index] != '"' && heap_file[start_index] != ' ' && heap_file[start_index] != '\n' && heap_file[start_index] != '\t')
			{
				temp->string_value[i] = heap_file[start_index];
				i++;
				put_space = 1;
			}
					
			else if (heap_file[start_index] != '"' && put_space)
			{
				//making the output look better

				temp->string_value[i] = '\n';
				i++;
				temp->string_value[i] = '\t';
				i++;
				put_space = 0;
			}


			if (heap_file[start_index] == '"')
				geresh_count++;

			if (heap_file[start_index] == '[' && geresh_count % 2 == 0)
				another_list++;

			if (heap_file[start_index] == ']' && geresh_count % 2 == 0)
				another_list--;


			start_index++;

			if (start_index == length)
			{
				printf("Syntax error! < missing -> ' ] ' >\n");
				return NULL;
			}
		}

		temp->string_value[i] = '\0'; //adding the null byte

		return temp;
	}
	//else is not needed because of the return


	temp->next = (struct Value*) malloc(sizeof(struct Value));
	temp->next->next = NULL;
	temp->next->placement = counter;
	temp->next->type = STRING;

	temp->next->string_length = value_length; //with the null byte
	
	temp->next->string_value = (char*) malloc(value_length);

	//making the output look better
	temp->next->string_value[i] = '\n';
	i++;
	temp->next->string_value[i] = '\t';
	i++;
	temp->next->string_value[i] = '[';
	i++;

	while (another_list > 0)
	{
		if (heap_file[start_index] != '"' && heap_file[start_index] != ' ' && heap_file[start_index] != '\n' && heap_file[start_index] != '\t')
		{
			temp->next->string_value[i] = heap_file[start_index];
			i++;
			put_space = 1;
		}
					
		else if (heap_file[start_index] != '"' && put_space)
		{
			//making the output look better

			temp->next->string_value[i] = '\n';
			i++;
			temp->next->string_value[i] = '\t';
			i++;
			put_space = 0;
		}


		if (heap_file[start_index] == '"')
			geresh_count++;

		if (heap_file[start_index] == '[' && geresh_count % 2 == 0)
			another_list++;

		if (heap_file[start_index] == ']' && geresh_count % 2 == 0)
			another_list--;


		start_index++;

		if (start_index == length)
		{
			printf("Syntax error! < missing -> ' ] ' >\n");
			return NULL;
		}
	}

	temp->next->string_value[i] = '\0'; //adding the null byte

	return temp->next;
}

struct Value* make_number_value(char* heap_file, int start_index, struct Value* temp, int length, int counter)
{
	int i = 0, float_num = 0, number_length = 0;

	number_length = number_value_length(heap_file, start_index); //with the null byte

	if (!temp)
	{
		temp = (struct Value*) malloc(sizeof(struct Value));
		temp->next = NULL;
		temp->placement = counter;
		temp->string_value = (char*) malloc(number_length); //with the null byte


		while (heap_file[start_index] != ' ' && heap_file[start_index] != ',' && heap_file[start_index] != '}' && heap_file[start_index] != '\n' && heap_file[start_index] != '\t')
		{
			temp->string_value[i] = heap_file[start_index];
			start_index++;
			i++;
					
			if (heap_file[start_index] == '.')
				float_num = 1;
		
			if (start_index == length)
			{
				printf("Syntax error! < problem with number value >\n");
				return NULL;
			}
		}

		temp->string_value[i] = '\0'; //adding the null byte in the end


		if (!float_num)	
		{ //if it's an integer
			temp->int_value = atoi(temp->string_value);
			temp->type = INTEGER;
		}

		else
		{ //if it's a double
			temp->double_value = atof(temp->string_value);
			temp->type = DOUBLE;
		}

		return temp;
	}
	//else is not needed because of the return	


	temp->next = (struct Value*) malloc(sizeof(struct Value));
	temp->next->next = NULL;
	temp->next->placement = counter;
	temp->next->string_value = (char*) malloc(number_length); //with the null byte

	while (heap_file[start_index] != ' ' && heap_file[start_index] != ',' && heap_file[start_index] != '}' && heap_file[start_index] != '\n' && heap_file[start_index] != '\t')
	{
		temp->next->string_value[i] = heap_file[start_index];
		start_index++;
		i++;
					
		if (heap_file[start_index] == '.')
			float_num = 1;
		
		if (start_index == length)
		{
			printf("Syntax error! < problem with number value >\n");
			return NULL;
		}
	}

	temp->next->string_value[i] = '\0'; //adding the null byte in the end


	if (!float_num)	
	{ //if it's an integer
		temp->int_value = atoi(temp->next->string_value);
		temp->type = INTEGER;
	}

	else
	{ //if it's a double
		temp->double_value = atof(temp->next->string_value);
		temp->type = DOUBLE;
	}

	return temp->next;
	
}

struct Tav* check_colon_list(char* heap_file, struct Tav* head, int length)
{
	struct Tav* temp = head;
	int i = 0, geresh_count = 0;

	while(1)
	{
		geresh_count = 0;	
		for (i = 0; i < temp->index; i++)
		{ //checking if the first : is okay
			if (heap_file[i] == '"')
				geresh_count++;
		}

		if (geresh_count % 2 != 0)
		{
			temp = temp->next;
			head = temp;
		}
		else
		{
			temp = temp->next;
			break;
		}
	}

	while (temp->next != NULL)
	{
		geresh_count = 0;

		for (i = temp->index; i < temp->next->index; i++)
		{
			if (heap_file[i] == '"')
				geresh_count++;
		}

		if (geresh_count % 2 != 0)
		{
			temp->next = temp->next->next;
		}

		temp = temp->next;

	}
	
	return head;
}

int valid_path(char* heap_file, int length, char* last_key_given, int keys_given/*the amount of keys entered*/, struct Key* head)
{
	struct Key* temp = head;
	int i = 0, j = 0, count_mesulsal = 0, keys_passed = 0, string_length = 0;
	char* ptr = NULL;

	while (1)
	{

		for (i = 0; i < length; i++)
		{

			if (heap_file[i] == '{')
				count_mesulsal++;

			if (heap_file[i] == '}')
				count_mesulsal--;

			if (heap_file[i] == '"')
			{
				j = 0;
				i++;
				string_length = string_value_length(heap_file, i); //with null byte

				ptr = (char*) malloc(string_length); 

				while(heap_file[i] != '"')
				{
					ptr[j] = heap_file[i];
					j++;
					i++;
				}
				ptr[j] = '\0';

			}

			if (ptr != NULL)
			{
				if (strcmp(ptr, temp->name) == 0) // checking if we didn't passed again on the same key - the name of the key could be also some value.....
				{
					if (strcmp(ptr, last_key_given) == 0)
					{
						if (!(keys_given - count_mesulsal)) //hefresh needs to be equals to 0
							return 1;

						return 0;
					}

					temp = temp->next;
				}

				free(ptr); //freeing to save some memory (:

				ptr = NULL;
			}
		}
	}
}

int string_value_length(char* heap_file, int start_index)
{
	int length = 0;

	while (heap_file[start_index] != '"')
	{
		length++;
		start_index++;
	}

	length++; //adding the null byte

	return length;
}

int object_value_length(char* heap_file, int start_index)
{
	int length = 0, count_mesulsal = 1;

	while (count_mesulsal != 0)
	{
		if (heap_file[start_index] == '{')
			count_mesulsal++;

		if (heap_file[start_index] == '}')
			count_mesulsal--;

		length++;

		start_index++;
	}

	length++; //adding the null byte

	return length;
}

int list_value_length(char* heap_file, int start_index)
{
	int length = 0, count_meruba = 1;

	while (count_meruba != 0)
	{
		if (heap_file[start_index] == '[')
			count_meruba++;

		if (heap_file[start_index] == ']')
			count_meruba--;

		length++;

		start_index++;
	}

	length++; //adding the null byte

	return length;
}

int number_value_length(char* heap_file, int start_index)
{
	int length = 0;

	while (heap_file[start_index] != ' ' && heap_file[start_index] != '\n' && heap_file[start_index] != '\t' && heap_file[start_index] != ',')
	{
		length++;
		start_index++;
	}

	length++; //adding the null byte

	return length;
}