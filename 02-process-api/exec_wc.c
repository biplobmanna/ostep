
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  printf("\n--- wc(pid: %d) ---\n", (int)getpid());
  fflush(stdout);
  char *args[3];
  args[0] = strdup("wc");
  if (argc > 1) {
    args[1] = argv[1];
  } else {
    args[1] = strdup("exec.c");
  }
  args[2] = NULL;
  execvp(args[0], args);
  printf("failed to execute 'wc'!\n");
  return 1;
}
