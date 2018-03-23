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

#include "demo.h"

int main() {
	node* head = create_node_int(0);
	node* last = head;
	printf("created head node: %s\n", sprint_node_int(head));

	for (int i = 1 ; i < 10 ; i ++) {
		node* new;
		printf("inserting %i... \n", i);
		new = create_node_int(i);
		append_node(last, new);
		last = new;
	}

	printf("List state is: \n");

	node* cursor = head;
	while (cursor != NULL) {
		printf("%s\n", sprint_node_int(cursor));
		cursor = get_next(cursor);
	}

	cursor = get_n_forward(head, 5);
	printf("The 5th node is: %s\n", sprint_node_int(cursor));
	printf("Deleting the 5th node... \n");
	delete_node(cursor);

	printf("List state is: \n");

	cursor = head;
	while (cursor != NULL) {
		printf("%s\n", sprint_node_int(cursor));
		cursor = get_next(cursor);
	}

	return 0;
}
