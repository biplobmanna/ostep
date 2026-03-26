
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void process() {
  int x = rand();
  int rc = fork();
  // x += 1;
  if (rc < 0) {
    fprintf(stderr, "failed to fork!\n");
    exit(1);
  } else if (rc == 0) {
    printf("running child (pid: %d)...\n", (int)getpid());
    printf("  c >> x = %d\n", x);
    x += 10;
    printf("  c >> x + 10 = %d\n", x);
  } else {
    wait(NULL);
    printf("parent (pid: %d) of child (pid: %d)...\n", (int)getpid(), rc);
    printf("  p >> x = %d\n", x);
  }
}

int main(int argc, char **argv) {
  process();
  return 0;
}
