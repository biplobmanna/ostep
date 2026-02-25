
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  printf("\n--- grep (pid: %d) ---\n", (int)getpid());
  char *args[6];
  args[0] = strdup("grep");
  args[1] = "-n";
  args[2] = "--color";
  args[3] = strdup("exec");
  args[4] = strdup("./exec.c");
  args[5] = NULL;
  execvp(args[0], args);
  printf("failed to execute 'grep'!\n");
  return 1;
}
