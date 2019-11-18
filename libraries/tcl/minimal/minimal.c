/* Authored by Charles A Daniels, 2019 */

/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

/* This file demonstrates how to instantiate a TCL interpreter within a C
 * program. Demonstrations of binding a C function to TCL, and sharing
 * variables between the two are shown.
 *
 * Note that you *probably* want to use the alternative style where your C
 * code is loaded as a TCL library. Especially if you want to use TCL as
 * an extension language for your program, you probably want that style.
 *
 * You should see ../library if you want to see an example.
 *
 * Really, if you want an extension language, you don't want to do it the
 * way this file shows. Trust me, I thought I knew better than all the
 * people that said that, then had to re-write a bunch of code.
 *
 * The style shown in this file is what you want if you're either doing
 * something very simple, or if you want to use this as some kind of embedded
 * configuration language. This *might* be what you want if you're trying to
 * build a DSL.
 */

#include <tcl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int g_time;

void alarm_handler(int signo) {
	/* run every second to update the time bound variable */
	g_time = time(NULL);
	alarm(1);
}

int fibo(int n) {
	if (n == 1) { return 1; }
	if (n == 2) { return 1; }
	else { return fibo(n-1) + fibo(n-2); }
}

int fibo_command(
	ClientData data,	/* can be used by libraries to pass an
				 * opaque pointer */
	Tcl_Interp* interp,	/* the interpreter to run in, sometimes you
				 * want to have more than one */
	int argc,		/* number of object arguments */
	Tcl_Obj* CONST* argv	/* list of object arguments */
	) {

	int n;
	Tcl_Obj* result;

	/* Note that this is a command that takes object, and sets an object as
	 * a result. If you're writing new TCL libraries, you should use this
	 * style, but old-school TCL operated on strings and "returned" as
	 * string.
	 *
	 * Also, keep in mind that the return is a success/fail status, the
	 * actual result is set as a pointer within the interpreter object, but
	 * you don't want to access that directly, you should use one of the
	 * SetResult commands.
	 */

	/* make sure there is *exactly* one argument */
	if (argc != 2) {
		/* display a wrong number of arguments error */
		Tcl_WrongNumArgs(interp, 0, argv, "usage: fibo N");

		/* signal that an error occurred */
		return TCL_ERROR;
	}

	/* Parse the argument as an int. This style is required because casting
	 * to int can fail, and we might need to catch and return the error.
	 *
	 * If you want a string, you should use Tcl_GetStringFromObj() which
	 * cannot fail, and directly returns a char*
	 */
	if (Tcl_GetIntFromObj(interp, argv[1], &n) != TCL_OK) {
		/* Note that this should preserve whatever error message
		 * GetIntFromObj set, since we aren't calling TCL_SetResult.
		 *
		 * The fact that we called GetIntFromObj is invisible from
		 * the perspective of TCL's call stack. */
		return TCL_ERROR;
	}

	/* this creates a new TCL object to store our integer result. This is
	 * *not* a bound variable, this is creating a duplicate. */
	result = Tcl_NewIntObj(fibo(n));

	/* Sets the result within the interpreter state so the caller can see
	 * it.  Older TCL code may set the pointer within the interp struct
	 * directly, but this should be avoided.
	 *
	 * Some old code will use Tcl_SetResult(), which is used to set a
	 * string result.
	 */
	Tcl_SetObjResult(interp, result);

	return TCL_OK;
}

int main(int argc, char** argv) {
	Tcl_Interp* interp;

	/* register the alarm handler as the signal handler */
	signal(SIGALRM, alarm_handler);

	/* wake us up in 1 second */
	alarm(1);

	/* this creates the interpreter object */
	interp = Tcl_CreateInterp();

	/* I'm not 100% clear on what this does, but it seems to be important.
	 * I think it helps with loading libraries */
	Tcl_InitStubs(interp, TCL_VERSION, 0);

	/* This instantiates our C function as a TCL function so it can be
	 * called. */

	Tcl_CreateObjCommand(
		interp,				/* The interpreter we wish to
						   operate on */
		"fibo",				/* The symbol name for the new
						   function within the
						   interpreter */
		fibo_command,			/* The C function pointer */
		NULL,				/* ClientData pointer */
		(Tcl_CmdDeleteProc*) NULL	/* Cleanup function for when
						   the procedure is deleted.
						   This is used in some styles
						   of "handles", but not
						   relevant to this example. */
	);

	/* This links the g_time variable as a read-only variable at the top
	 * level of our TCL interpreter */
	Tcl_LinkVar(
		interp,			/* interpreter to create linked var in
					 */
		"linked_time",		/* symbol name */
		(char*) &g_time,	/* pointer to shared variable, has 
					 * to be of type char* for historical
					 * reasons, use flags to set a
					 * different type if appropriate */
		TCL_LINK_INT | TCL_LINK_READ_ONLY	/* flags */
	);

	if (isatty(fileno(stdin))) {
		/* if standard input is an interactive terminal, then use
		 * readline to make line editing nice */
		char* line = NULL;

		/* keep looping, when readline is done it will return NULL */
		while ((line = readline("minimal> ")) != NULL) {

			/* ignore empty lines */
			if (strlen(line) > 0) {

				/* enable up arrow to work */
				add_history(line);

				/* execute the line of input */
				Tcl_Eval(interp, line);

				/* display the result */
				printf("%s\n", Tcl_GetStringResult(interp));

			}
		}

	} else {
		/* if standard in is non-interactive, don't fuss with line
		 * editing or history */
		Tcl_EvalFile(interp, "/dev/stdin");

		/* and display the results */
		printf("%s\n", Tcl_GetStringResult(interp));
	}
}
