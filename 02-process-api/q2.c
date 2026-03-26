// Write a program that opens a file (with the open() system call) and then
// calls fork() to create a new process. Can both the child and parent ac-
// cess the file descriptor returned by open()? What happens when they are
// writing to the file concurrently, i.e., at the same time?

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *contents[7] = {
    "--------------------------------------------------\n",
    "Line1: From time immemorial, life has always been \n",
    "Line2: somewhat difficult and easy at the same time, \n",
    "Line3: yet, there were always species that thrived. \n",
    "Line4: These species evolved with time, while others \n",
    "Line5: became extinct. Survival of the fittest.\n",
    "--------------------------------------------------\n",
};

void write_to_file(int fd, char parent_or_child) {
  int pid = (int)getpid();
  char buffer[256];
  for (int i = 0; i < 7; ++i) {
    // add contents to buffer
    snprintf(buffer, sizeof(buffer), "%c [pid: %d] %s", parent_or_child, pid,
             contents[i]);

    // write buffer to file
    write(fd, buffer, strlen(buffer));

    // see what's happening in real-time
    fprintf(stderr, "%s", buffer);

    // sleep delay to make concurrent writing more apparent
    usleep(100000);
  }
}

int main(int argc, char **argv) {
  // open file in write and trunc mode
  // create with permissions: 0x744
  int fd =
      open("q2.out", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU | S_IRGRP | S_IROTH);
  fprintf(stderr, "[pid: %d] file opened (fd: %d)\n", (int)getpid(), fd);

  // fork
  int rc = fork();
  if (rc < 0) {
    exit(1);
  } else if (rc == 0) { // child
    printf("[pid: %d] Child started writing to file...\n", (int)getpid());
    write_to_file(fd, 'C');
    printf("[pid: %d] Child finished writing to file...\n", (int)getpid());
  } else { // parent
    printf("[pid: %d] Parent started writing to file...\n", rc);
    write_to_file(fd, 'P');
    printf("[pid: %d] Parent started writing to file...\n", rc);
  }

  return 0;
}
