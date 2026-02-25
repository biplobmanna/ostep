
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  // fork the process, and allow for redirection
  int rc = fork();

  if (rc < 0) {
    fprintf(stderr, "failed to fork\n");
    exit(1);
  } else if (rc == 0) {
    close(STDOUT_FILENO);
    open("./redirect.out", O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
    // exec wc
    char *args[3];
    args[0] = "./exec_wc";
    args[1] = "exec_redirect.c";
    args[2] = NULL;
    execvp(args[0], args);
    fprintf(stderr, "failed to run execvp\n");
    exit(2);
  } else {
    int rc_wait = wait(NULL);
    printf(
        "return to parent (pid: %d) waiting (rc_wait: %d) for child (pid: %d)",
        (int)getpid(), rc_wait, rc);
  }
  return 0;
}
