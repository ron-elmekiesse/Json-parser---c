#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define NAME_SIZE 100

enum TYPES
{
	STRING = 0,
	INTEGER = 1,
	DOUBLE = 2
};

struct Tav
{
	int index;
	struct Tav* next;
};

struct Key
{
	int placement;
	char* name;
	int name_length;
	struct Key* next;
};

struct Value
{
	int placement;
	//char string_value[NAME_SIZE];
	char* string_value;
	int string_length;
	int int_value;
	double double_value;
	enum TYPES type;
	struct Value* next;
};

// this is the module...

//return the file lenght
int get_file_length(char* file_name);

//reads the ffile content to the allocated heap memory
int read_file_to_heap(FILE* fname, int length, char* ptr_heap);

//prints out a Linked List
int print_Tav_list(struct Tav* head);

//resetting a char array
void reset_char_array(char* arr, int length);