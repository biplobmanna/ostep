
// AIM:
// P1 --> P2 --> cat
//    --> P3 --> wc
//    --> P4 --> grep
// use exec to transform the child process into a new process
// while keeping the parent intact

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int rc;
  for (int i = 0; i < 3; i++) {
    rc = fork();
    if (rc < 0) {
      fprintf(stderr, "failed to fork! (i: %d)\n", i);
      exit(1);
    } else if (rc == 0 && i == 0) {
      printf("\nstarting 'exec_cat' (pid: %d) @ i=%d", (int)getpid(), i);
      char *args[2];
      args[0] = "./exec_cat";
      args[1] = NULL;
      execvp(args[0], args);
      printf("failed to exec 'exec_cat'!");
      exit(1);
    } else if (rc == 0 && i == 1) {
      printf("\nstarting 'exec_wc' (pid: %d) @ i=%d\n", (int)getpid(), i);
      char *args[2];
      args[0] = "./exec_wc";
      args[1] = NULL;
      execvp(args[0], args);
      printf("failed to exec 'exec_wc'!");
      exit(1);
    } else if (rc == 0 && i == 2) {
      printf("\nstarting 'exec_grep' (pid: %d) @ i=%d\n", (int)getpid(), i);
      char *args[2];
      args[0] = "./exec_grep";
      args[1] = NULL;
      execvp(args[0], args);
      printf("failed to exec 'exec_grep'!");
      exit(1);
    } else {
      wait(NULL);
      printf("\nreturn to parent (pid: %d) @ i=%d from child (pid: %d)\n",
             (int)getpid(), i, rc);
    }
  }
  return 0;
}
