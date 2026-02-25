#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argv, char *argc[]) {
  printf("Hello from process (pid: %d)\n", (int)getpid());

  // create multiple processes in a loop
  for (int i = 0; i < 10; i++) {
    // fork the process from here
    int rc = fork();
    if (rc < 0) {
      fprintf(stderr, "[Loop: %d] fork failed!\n", i);
      exit(1);
    } else if (rc == 0) {
      printf("[Loop: %d] Hello from child (pid: %d)\n", i, (int)getpid());
      break;
    } else {
      int rc_wait = wait(NULL);
      printf(
          "[Loop: %d] Hello from parent (pid: %d), waiting (rc_wait: %d) for "
          "child (pid: %d)\n",
          i, rc_wait, (int)getpid(), rc);
    }
  }
  return 0;
}
