#include <stdio.h>
#include <unistd.h>
int main(int argc, char **argv) {
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "[P: %d] failed to fork!\n", (int)getpid());
    return 1;
  } else if (rc == 0) {
    // child process work
    close(STDOUT_FILENO);
    printf("[C: %d] working...\n", (int)getpid());
  } else {
    // parent process work
    printf("[P: %d] working...\n", (int)getpid());
  }
  return 0;
}
