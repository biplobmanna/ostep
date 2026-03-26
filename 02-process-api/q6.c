

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void parent_work() { printf("[P: (pid: %d)] Working...\n", (int)getpid()); }
void child_work() { printf("[C: (pid: %d)] Working...\n", (int)getpid()); }

int main(int argc, char **argv) {
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "[P, %d] failed to fork!", (int)getpid());
    return 1;
  } else if (rc == 0) {
    child_work();
  } else {
    int rc_wait = waitpid(rc, NULL, 0);
    printf("[P: (pid: %d)] waiting for child (rc_wait: %d)...\n", (int)getpid(),
           rc_wait);
    parent_work();
  }
  return 0;
}
