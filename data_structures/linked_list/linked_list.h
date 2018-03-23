#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	void* prev;
	void* next;
	void* data;
} node;

#define SPRINT_BUF_SIZE 2048

node* create_node(void* data);
void insert_node(node* prev, node* new, node* next);
void append_node(node* new, node* where);
void prepend_node(node* new, node* where);
void delete_node(node* where);
node* get_next(node* where);
node* get_prev(node* where);
node* get_n_forward(node* where, int n);
node* get_n_back(node* where, int n);
char* sprint_node(node* what);
char* sprint_node_int(node* what);
node* create_node_int(int data);

#endif
