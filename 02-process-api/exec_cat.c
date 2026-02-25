#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  printf("\n--- cat (pid: %d)---\n", (int)getpid());
  char *args[3];
  args[0] = strdup("cat");
  args[1] = strdup("exec.c");
  args[2] = NULL;
  execvp(args[0], args);
  printf("failed to execute 'cat'!\n");
  return 1;
}
