// AIM:
// P1 --> P2 --> cat
//    --> P3 --> wc
//    --> P4 --> grep
// use exec to transform the child process into a new process
// while keeping the parent intact

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void exec_cat() {
  printf("\n--- cat ---\n");
  char *argv[3];
  argv[0] = strdup("cat");
  argv[1] = strdup("exec.c");
  argv[2] = NULL;
  execvp(argv[0], argv);
  printf("failed to execute 'cat'!\n");
}

void exec_wc() {
  printf("\n--- wc ---\n");
  char *argv[3];
  argv[0] = strdup("wc");
  argv[1] = strdup("exec.c");
  argv[2] = NULL;
  execvp(argv[0], argv);
  printf("failed to execute 'wc'!\n");
}

void exec_grep() {
  printf("\n--- grep ---\n");
  char *argv[4];
  argv[0] = strdup("grep");
  argv[1] = strdup("exec");
  argv[2] = strdup("exec.c");
  argv[3] = NULL;
  execvp(argv[0], argv);
  printf("failed to execute 'grep'!\n");
}

int main(int argc, char **argv) {
  int rc;
  for (int i = 0; i < 3; i++) {
    rc = fork();
    if (rc < 0) {
      fprintf(stderr, "failed to fork! (i: %d)\n", i);
      exit(1);
    } else if (rc == 0 && i == 0) {
      printf("\nstarting 'cat' @ i=%d", i);
      exec_cat();
    } else if (rc == 0 && i == 1) {
      printf("\nstarting 'wc' @ i=%d\n", i);
      exec_wc();
    } else if (rc == 0 && i == 2) {
      printf("\nstarting 'grep' @ i=%d\n", i);
      exec_grep();
    } else {
      wait(NULL);
      printf("\nreturn to parent @ i=%d\n", i);
    }
  }
  return 0;
}
