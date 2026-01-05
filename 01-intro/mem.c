#include "common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int *p;                  // init a integer pointer
  p = malloc(sizeof(int)); // allocate 4b of memory
  assert(p != NULL);       // assert allocation

  if (argc < 2) {
    fprintf(stderr, "usage: mem <value>\n"); // fprintf takes a FILE pointer to
                                             // print to; printing to STDERR
    fprintf(stdout, "defaulting to: mem 0\n"); // printing to STDOUT
    *p = 0;
  } else if (argc == 2) {
    *p = atoi(argv[1]); // assign value to addr stored in p
    // if anything other than valid integer is provided, then defaults to 0, or
    // -1 if alphabet, defaults to 0 if out of bounds int, default to -1 if
    // float, double, truncates to int
  } else if (argc >= 3) {
    fprintf(stderr, "usage: mem <value>\n");
    fprintf(stdout, "utilising only the first arg...\n");
    fprintf(stdout, "running: mem %s\n",
            argv[1]);   // the argv params are of type char*
    *p = atoi(argv[1]); // assign value to addr stored in p
  }

  printf("(%d) addr pointed to by p: %p\n", (int)getpid(),
         p);                                     // print PID and mem LOC
  printf("(%d) value of p: %d\n", getpid(), *p); // print the value
  while (1) {
    Spin(1);                                       // Sleep for 1 secs
    *p = *p + 1;                                   // INC value inside p
    printf("(%d) value of p: %d\n", getpid(), *p); // print the value
  }

  return 0;
}
