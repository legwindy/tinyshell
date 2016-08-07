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
char *next_line(char *prompt, FILE *fp) {
	int c;        /*input char*/
	char *line_buf;        /*the buffer to store one line*/
	int buf_size = 0;
	int pos = 0;

	printf("%s", prompt);
	while((c = getc(fp)) != EOF) {
		if(buf_size == 0) {
			line_buf = emalloc(BUFSIZ);
		}else if(pos + 1 >= buf_size / sizeof(char)) {
			line_buf = erealloc(line_buf, buf_size + BUFSIZ);
		}
		buf_size += BUFSIZ;

		if(c == '\n') {
			break;
		}

		line_buf[pos++] = c;
	}

	if(c == EOF && pos == 0) {
		return NULL;
	}

	line_buf[pos] = '\0';
	return line_buf;
}

/*
 * porpose: split a line into an array of commands
 * example: "who;pwd" => ["who", "pwd"]
 */
char **splitline(char *linebuf) {
	int array_size = BUFSIZ;
	char **cmd_array = (char**)malloc(array_size);
	int cmd_num = 0;
	char *delim = ";";
	cmd_array[cmd_num] = strtok(linebuf, delim);
	while(cmd_array[cmd_num] != NULL) {
		if(cmd_num + 1 >= array_size / sizeof(char*)) {
			cmd_array = (char**)realloc(cmd_array, array_size + BUFSIZ);
			array_size += BUFSIZ;
		}
		cmd_array[++cmd_num] = strtok(NULL, delim);
	}

	//cmd_array[cmd_num] = NULL;

	return cmd_array;
} 

/*
 * purpose: split a cmd into array of white-space seperate tokens
 */
char **splitcmd(char *cmd_buf) {
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
			if(pos + 1 >= argbuf_size / sizeof(char*)) {
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
		arglist = (char**)malloc(sizeof(void*));
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
