#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void parent_work() { printf("[P: (pid: %d)] Working...\n", (int)getpid()); }
void child_work() { printf("[C: (pid: %d)] Working...\n", (int)getpid()); }

int main(int argc, char **argv) {
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "[ERR] failed to fork...\n");
    return 1;
  } else if (rc == 0) {
    int wait_rc = wait(NULL);
    if (wait_rc < 0) {
      fprintf(stderr, "[C: (pid %d)] Failed to wait: [ERRNO: %d]", rc, errno);
    } else {
      printf("[C: (pid: %d)] Waiting (wait_rc: %d)...\n", rc, wait_rc);
    }
    child_work();
  } else {
    int wait_rc = wait(NULL);
    printf("[P: (pid: %d)] Waiting (wait_rc: %d)...\n", (int)getpid(), wait_rc);
    parent_work();
  }
  return 0;
}
