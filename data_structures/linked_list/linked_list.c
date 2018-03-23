/* Copyright (c) 2018, Charles Daniels
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include "linked_list.h"

/* Simple double linked list implementation */

node* create_node(void* data) {
	node* n = (node*) malloc(sizeof(node));
	n->prev = NULL;
	n->data = data;
	n->next = NULL;
	return n;
}

void insert_node(node* prev, node* new, node* next) {
	prev->next = new;
	new->prev = prev;

	next->prev = new;
	new->next = next;
}

void append_node(node* where, node* new) {
	if (where->next == NULL) {
		where->next = new;
		new->prev = where;
	} else {
		node* next = get_next(where);
		insert_node(where, new, next);
	}
}

void prepend_node(node* new, node* where) {
	if (where->prev == NULL){
		where->prev = new;
		new->next = where;
	} else {
		node* prev = get_prev(where);
		insert_node(prev, new, where);
	}
}

void delete_node(node* where) {
	node* prev = get_prev(where);
	node* next = get_next(where);

	if ((prev != NULL) && (next != NULL)) {
		prev->next = next;
		next->prev = prev;
	} else if (prev != NULL) {
		prev->next = NULL;
	} else if (next != NULL) {
		next->prev = NULL;
	}

	free(where);
}

node* get_next(node* where) {
	return where->next;
}

node* get_prev(node* where) {
	return where->prev;

}

node* get_n_forward(node* where, int n) {
	if (n <= 0) {
		return where;
	} else {
		return get_n_forward(get_next(where), n - 1);
	}
}

node* get_n_back(node* where, int n) {
	if (n <= 0) {
		return where;
	} else {
		return get_n_back(get_prev(where), n - 1);
	}
}

char* sprint_node(node* what) {
	char* buf = (char*) malloc(sizeof(char) * SPRINT_BUF_SIZE);
	sprintf(buf, "%p -> %p -> %p", (void*) what->prev, (void*) what,
					 (void*) what->next);
	return buf;
}

char* sprint_node_int(node* what) {
	char* buf = (char*) malloc(sizeof(char) * SPRINT_BUF_SIZE);
	int val = * ((int*) what->data);
	sprintf(buf, "%p -> %p (%i) -> %p", (void*) what->prev,
					      (void*) what, val,
					      (void*) what->next);
	return buf;

}

node* create_node_int(int data) {
	int* n = (int*) malloc(sizeof(int));
	*n = data;
	return create_node((void*) n);
}

