/*
 * tinyshell.c - small shell version 1
 * tinyshell parses the command line into strings
 * uses fork, exec,wait and ignore signals
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "tinyshell.h"

#define DFL_PROMPT "> "

void setup();

int main() {
	char *linebuf, *prompt, **arglist;
	char **cmd_array;
	int result;

	prompt = DFL_PROMPT;
	setup();

	while((linebuf = next_line(prompt, stdin)) != NULL) {
		cmd_array = splitline(linebuf);
		int i = 0;
		while(cmd_array[i] != NULL) {
			if((arglist = splitcmd(cmd_array[i])) != NULL) {
				result = execute(arglist);
			}
			i++;
		}
		freelist(cmd_array);
	}
	return 0;
}

/*
 * purpose: initialize shell
 */
void setup() {
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n) {
	fprintf(stderr, "Error: %s, %s\n", s1, s2);
	exit(n);
}
