#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// this is the module...

//making a Linked List of an input char - returns the head of the linked list
struct Tav* make_list(char* heap_file, int length, char to_link);

//making a linked list of the keys - returns the head
struct Key* make_keys_list(char* heap_file, int length);

//making a linked list of the value - returns the head
struct Value* make_values_list(char* heap_file, int length);

//adding a string value to the linked list - return NULL error happened
struct Value* make_string_value(char* heap_file, int start_index, struct Value* temp, int length, int counter);

//adding an object value to the linked list - return NULL error happened
struct Value* make_object_value(char* heap_file, int start_index, struct Value* temp, int length, int counter);

//adding a list value to the linked list - return NULL error happened
struct Value* make_list_value(char* heap_file, int start_index, struct Value* temp, int length, int counter);

//adding an int or a double value to the linked list - return NULL error happened
struct Value* make_number_value(char* heap_file, int start_index, struct Value* temp, int length, int counter);

//checking if the : are not fake - if it is they will be removed, returns the head of the linked list
struct Tav* check_colon_list(char* heap_file, struct Tav* head, int length);

//checking if the entered path is the full path to the value, returns 1 if it's a valid path
int valid_path(char* heap_file, int length, char* command, int keys_given, struct Key* head);

//returns the length of the string value, with the null byte
int string_value_length(char* heap_file, int start_index);

//returns the length of the object value, with the null byte
int object_value_length(char* heap_file, int start_index);

//returns the length of the list value, with the null byte
int list_value_length(char* heap_file, int start_index);

//returns the length of the number value, with the null byte
int number_value_length(char* heap_file, int start_index);

//checking if the user asked for a specific index in list, example: list[0] - returns 1 if it is
int specific_list_index(char* command);