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

/* This file demonstrates how to implement a TCL package using C. In general,
 * this is the preferred method of accessing C code from within TCL. */

#include <tcl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

static int g_time;

static void alarm_handler(int signo) {
	/* run every second to update the time bound variable */
	g_time = time(NULL);
	alarm(1);
}

static int fibo(int n) {
	if (n == 1) { return 1; }
	if (n == 2) { return 1; }
	else { return fibo(n-1) + fibo(n-2); }
}

static int fibo_command(
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

int DLLEXPORT Samplepackage_Init(Tcl_Interp *interp) {
	Tcl_Namespace *nsPtr; /* pointer to hold our own new namespace */

	/* register the alarm handler as the signal handler */
	signal(SIGALRM, alarm_handler);

	/* wake us up in 1 second */
	alarm(1);

	/* as in the other example, I'm not entirely clear what this does, but
	 * it seems to make package importing work better */
	if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
		return TCL_ERROR;
	}

	/* create the namespace object for our package */
	nsPtr = Tcl_CreateNamespace(interp, "samplepackage", NULL, NULL);
	if (nsPtr == NULL) {
		return TCL_ERROR;
	}

	/* This instantiates our C function as a TCL function so it can be
	 * called. */
	Tcl_CreateObjCommand(
		interp,				/* The interpreter we wish to
						   operate on */
		"samplepackage::fibo",		/* The symbol name for the new
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
		"samplepackage::linked_time",	/* symbol name */
		(char*) &g_time,	/* pointer to shared variable, has 
					 * to be of type char* for historical
					 * reasons, use flags to set a
					 * different type if appropriate */
		TCL_LINK_INT | TCL_LINK_READ_ONLY	/* flags */
	);

	/* Allow fibo and linked_time to be pulled in via namespace import...
	 * you would want to NOT do this if you intend for function
	 * to be private to the namespace. This is equivelant to the 'namespace
	 * export ...' TCL command */
	Tcl_Export(interp, nsPtr, "fibo", 0);

	/* Note that Tcl_Export does not work for variables, hence linked_time
	 * cannot be included via namespace import directly */

	Tcl_PkgProvide(interp, "samplepackage", "0.0.1");

	return TCL_OK;

}
