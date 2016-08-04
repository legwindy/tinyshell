/*
 * splitline.c - reads commands and parses functions for tinyshell
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyshell.h"

#define is_delim(x) ((x) == ' ' || (x) == '\t')

/*
 * purpose: read next command line from fp
 */
char *next_cmd(char *prompt, FILE *fp) {
	int c;        /*input char*/
	char *cmd_buf;        /*the buffer to store one line*/
	int buf_size = 0;
	int pos = 0;

	printf("%s", prompt);
	while((c = getc(fp)) != EOF) {
		if(buf_size == 0) {
			cmd_buf = emalloc(BUFSIZ);
		}else if(pos + 1 >= buf_size) {
			cmd_buf = erealloc(cmd_buf, buf_size + BUFSIZ);
		}
		buf_size += BUFSIZ;

		if(c == '\n') {
			break;
		}

		cmd_buf[pos++] = c;
	}

	if(c == EOF && pos == 0) {
		return NULL;
	}

	cmd_buf[pos] = '\0';
	return cmd_buf;
}

/*
 * purpose: split a line into array of white-space seperate tokens
 */
char **splitline(char *cmd_buf) {
	char **arglist = NULL;        /*arglist: 参数指针数组*/
	int arglist_size = 0;        /*the total memory space of arglist*/
	int argnum = 0;        /*the number of arguments*/

	char *argbuf = NULL;        /*argbuf: 用于单个参数*/
	int argbuf_size = 0;        /*the total memory space of argbuf*/
	int pos = 0;        /*当前参数的长度*/

	char *cp = cmd_buf;
	while((*cp != '\0')) {
		while(is_delim(*cp)) {
			cp++;
		}

		if(*cp == '\0') {
			break;
		}
		
		pos = 0;
		argbuf_size = 0;
		while(!is_delim(*cp)) {
			if(pos + 1 >= argbuf_size) {
				if(argbuf_size == 0) {
					argbuf = (char*)malloc(BUFSIZ);
				}else {
					argbuf = (char*)realloc(argbuf, argbuf_size + BUFSIZ);
				}
				argbuf_size += BUFSIZ;
			}
			argbuf[pos++] = *cp;
			cp++;
		}
		argbuf[pos] = '\0';

		if(argnum + 1 >= arglist_size) {
			if(arglist_size == 0) {
				arglist = (char**)malloc(BUFSIZ);
			}else {
				arglist = (char**)realloc(arglist, arglist_size + BUFSIZ);
			}
			arglist_size += BUFSIZ;
		}
		arglist[argnum++] = argbuf;
	}
	
	if(argnum == 0) {        /*当直接输入'\n'时，需要分配内存*/
		arglist = (char**)malloc(1);
	}
	arglist[argnum] = NULL;

	return arglist;
}

/*
 * purpose: constructor for strings
 */
char *newstr(char *s, int l) {
	char *rv = emalloc(l + 1);
	rv[l] = '\0';
	strncpy(rv, s, l);
	return rv;
}

/*
 * purpose: free the list returnd by splitline
 */
void freelist(char **list) {
	char **cp = list;
	while(*cp) {
		free(*cp++);
	}
	free(list);
}

void *emalloc(size_t n) {
	void *rv;
	if((rv = malloc(n)) == NULL) {
		fatal("out of memory", "", 1);
	}
	return rv;
}

void *erealloc(void *p, size_t n) {
	void *rv;
	if((rv = realloc(p, n)) == NULL) {
		fatal("realloc() failed", "", 1);
	}
	return rv;
}
