/**
 * @brief A read–eval–print loop (REPL), also known as an interactive toplevel or language shell, is a simple, interactive computer
 * programming environment that takes single user inputs (i.e. single expressions), evaluates them, and returns the result to the user;
 * a program written in a REPL environment is executed piecewise.
 *
 * @author	Artjom Siebert
 * @date	29.01.2015
 */

#include "ETRX357.h"
#include <stdio.h>
#include <stdin.h>

int ReadEvalPrintLoop(void);
char* readLine(void);

#define EXIT	"BYE"

char* readLine(void){
	char prompt[512];	//max reading characters

	gets(prompt);
}

int ReadEvalPrintLoop(void){
	char *p;
	printf(">");
	while((p = readLine()) != EXIT){
		printf(" %s\n",p);
		printf(">");
	}
}
