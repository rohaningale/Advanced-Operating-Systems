/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - print Hello <string>, Welcome to the world of Xinu!!
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {

	/* Check argument count */

	if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "$ hello <string>\n");
		fprintf(stderr,"<string> with multiple token words can be placed in double quotations like \" Sam Johnson \" \n");
		return 1;
	}

	if(nargs < 2) {
		fprintf(stderr,"%s Too less arguments\n", args[0]);
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "$ hello <string>\n");
		return 1;
	}

	/* Valid arguments passed */

	printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
	return 0;
	
}
