// Write a program that calls fork() and then calls some form of exec() to
// run the program /bin/ls. See if you can try all of the variants of exec(),
// including (on Linux) execl(), execle(), execlp(), execv(), execvp(),
// and execvpe(). Why do you think there are so many variants of the same
// basic call?

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void call_ls() {

  // printf("-- execl(): --- \n");
  // execl("/bin/ls", "ls", "-lah", "../", NULL);

  char *args[4];
  args[0] = "ls";
  args[1] = "-lah";
  args[2] = "../";
  args[3] = NULL;
  // printf("--- execv(): ---\n");
  // execv("/bin/ls", args);

  char *env[] = {"CUSTOM_VAL=Hello from execle()!", "DIR_PATH=..", NULL};
  // execle("/usr/bin/sh", "sh", "-c", "echo $CUSTOM_VAL; ", NULL, env);

  // execlp("ls", "ls", "-lah", "../", NULL);

  // execv("/bin/ls", args);

  execvp(args[0], args);

  // cannot be called on UNIX systems on later C versions
  // args[2] = "$DIR_PATH";
  // execvpe(args[0], args, env);
}

int main(int argc, char **argv) {
  int rc = fork();

  if (rc < 0) {
    exit(1);
  }
  if (rc == 0) {
    call_ls();
  } else {
    // do nothing here
  }
  return 0;
}
