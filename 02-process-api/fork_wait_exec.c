#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("hello world (pid: %d)\n", (int)getpid());
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("child (pid: %d)\n", (int)getpid());

    // start a new process with exec
    char *myargs[3];
    myargs[0] = strdup("wc");
    myargs[1] = strdup("fork_wait_exec.c");
    myargs[2] = NULL;
    printf("\n--- EXEC ---\n");
    execvp(myargs[0], myargs);
    printf("this line is unreachable...");
  } else {
    int rc_wait = wait(NULL);
    printf("parent:(pid: %d), waiting:(rc_wait: %d), child:(rc_wait: %d)\n",
           (int)getpid(), rc_wait, rc);
  }
  return 0;
}
