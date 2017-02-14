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


#define NBPIPES 10
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

void closePipes(int p[NBPIPES][2], int nbPipes) {
  for (int i = 0; i < nbPipes - 1; ++i) {
    close(p[i][0]);
    close(p[i][1]);
  }
}
int main() {
  while (1) {
    struct cmdline *l;
    int seq_len = 0,
        status;

    int p[NBPIPES][2];
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

    seq_len = seqLength(l->seq);
    if (seq_len > 1) {
      for (int i = 0; i < seq_len - 1; ++i){
        xPipe(p[i]);
      }
    }

    /* Execute each command of the pipe */
    for (int i=0; l->seq[i]!=0; i++) {
      char **cmd = l->seq[i];
      if (strcmp(cmd[0],"exit")){
        pid = fork();
        if (pid == 0) {
          if (seq_len > 1) {
            if (i == 0) {
              dup2(p[i][1], STDOUT_FILENO);
            }
            else if (i == seq_len - 1){
              dup2(p[i-1][0], STDIN_FILENO);
            }
            else {
              dup2(p[i-1][0], STDIN_FILENO);
              dup2(p[i][1], STDOUT_FILENO);
            }
          }
          
          closePipes(p, seq_len);
          execvp(cmd[0], cmd);
          exit(0); // Exit if cmd[0] is not a  correct shell command
        }
        else {
          close(p[i][1]); 
          for (int i = 0; l->seq[i] != 0; ++i) {
            waitpid(-1, &status, 0); // TODO: Récupération/Gestion des status
          }
        }
      }
      else {
        exit(0);
      }
    }
  }
}

