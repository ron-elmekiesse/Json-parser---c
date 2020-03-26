#include "utilities.h"

int get_file_length(char* file_name)
{
	int length = 0;
	FILE* fp = NULL;

	fp = fopen(file_name, "r"); //Always!!!! Check the return....

	if(!fp){
		printf("No such file!\n");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	
	length = ftell(fp);

	//printf("The file length is %d \n", length);
	
	fclose(fp);
	
	return length;
}

int read_file_to_heap(FILE* fname, int length, char* ptr_heap)
{
	fread(ptr_heap, sizeof(char), length, fname);

	return 0;
}

int print_Tav_list(struct Tav* head)
{
	struct Tav* temp = head;

	while (temp != NULL)
	{
		printf("index = %d \n", temp->index);
		temp = temp->next;
	}
	return 0;
}

void reset_char_array(char* arr, int length)
{
	int i = 0;

	for (i = 0; i < length; i++)
	{
		arr[i] = '\0';
	}
}