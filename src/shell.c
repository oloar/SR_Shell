/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "readcmd.h"


int main()
{
	while (1) {
		struct cmdline *l;
		int i,
			status;
		pid_t pid;

		printf("> ");
		l = readcmd();
		/* If input stream closed, normal termination */
		if (!l) {
			printf("exit\n");
			exit(0);
		}

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		/* Display each command of the pipe */
		for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			if (strcmp(cmd[0],"exit")){
				pid = fork();
				if (pid == 0) {
					execvp(cmd[0], cmd);
				}
				else {
					waitpid(pid, &status, 0);// TODO: Récupération/Gestion des status
				}
				printf("\n");
			}
			else {
				exit(0);
			}
		}
	}
}
