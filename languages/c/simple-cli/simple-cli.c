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

/*
 * This program reads in integers from standard input, performs a simple
 * arithmetic operation on them, then outputs the result to standard out.
 *
 * It is intended to demonstrate correct handling of standard input, standard
 * output, and argument handling.
 */

#include "simple-cli.h"

int main(int argc, char** argv) {

	int flag_s;
	int flag_q;
	char opt;
	char* line;
	size_t line_len;
	int line_val;

	line_len = 0;
	flag_s = 0;
	flag_q = 1;

	while ((opt = getopt (argc, argv, "sq:h")) != -1) {
		switch (opt) {
			case 's':
				flag_s = 1;
				break;
			case 'q':
				/* note that atoi() is a bad way to do this
				 * because it has undefined behavior if
				 * the argument cannot be converted */
				flag_q = atoi(optarg);
				break;
			case 'h':
				printf("Read integers from standard input, ");
				printf("add the specified\nquantity ");
				printf("to each number and print it ");
				printf("to standard out.\n\n");

				printf("-s . . . . . Subtract the quantity");
				printf("instead of adding it.\n\n");

				printf("-q [int] . . Specify quantity (");
				printf("default: 1).\n\n");

				printf("-h . . . . . display this message.\n");

		}

	}

	while(getline(&line, &line_len, stdin) >= 0) {

		/* remove trailing newline */
		for (int i = line_len; i >= 0; i--){
			if (line[i] == '\n') {
				line[i] = '\0';
				break;
			}
		}

		/* note that this is unsafe, as atoi has undefined behavior
		 * on error */
		line_val = atoi(line);

		printf("%i\n", line_val + ((flag_s) ? -1 : 1) * (flag_q));

	}

	return 0;

}
