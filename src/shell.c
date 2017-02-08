/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "readcmd.h"
/*
 * stdin  0
 * stdout 1
 * stderr 2
 *
 * pipe[0] read
 * pipe[1] write
 */

/* Pipe stuff */
void xPipe(int *p) {
	if(pipe(p) < -1){
		fprintf(stderr, "Erreur lors de l'ouverture du pipe.\n");
		exit(2);
	}
}

int seqLength(char ***seq) {
	int i = 0;
	while(seq[i] != 0)
		i++;
	return i;
}

int main() {
	while (1) {
		struct cmdline *l;
		int i,
			switchInOut = 0,
			seq_len = 0,
			status,
			p[2];
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

		xPipe(p);
		seq_len = seqLength(l->seq);

		/* Execute each command of the pipe */
		for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			if (strcmp(cmd[0],"exit")){

				pid = fork();
				if (pid == 0) {
					// if (i == seq_len - 1) {
					if (switchInOut) {
						dup2(p[0], 0);
						/* TODO: Gestion des pipes successifs
						 * Deuxième tube nécéssaire.
						 */
					}
					else {
						dup2(p[1], 1);
						// close(p[0]);
					}
					execvp(cmd[0], cmd);
					exit(0); // Exit if cmd[0] is not a  correct shell command
				}
				else {
					switchInOut = (switchInOut + 1) % 2;
					close(p[1]);
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

