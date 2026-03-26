// Write another program using fork(). The child process should print “hello”;
// the parent process should print “goodbye”. You should try to ensure that
// the child process always prints first; can you do this without calling wait()
// in the parent?

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int rc = fork();

  if (rc < 0) {
    exit(1);
  } else if (rc == 0) {
    printf("[child: (pid: %d)] hello\n", (int)getpid());
  } else {
    usleep(100);
    printf("[parent: (pid: %d) goodbye\n", rc);
  }

  return 0;
}
