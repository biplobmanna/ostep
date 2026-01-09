#include "common_threads.h"
#include <stdio.h>
#include <stdlib.h>

// global variables
volatile int counter = 0;
int loops;

void *worker(void *arg) {
  int i;
  for (i = 0; i < loops; i++) {
    counter++;
    // fprintf(stdout, "counter : %d\n", counter);
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  // default usage
  if (argc != 2) {
    fprintf(stderr, "usage: threads <loops>\n");
    exit(1);
  }
  // number of loops
  loops = atoi(argv[1]);

  // declare the threads
  pthread_t p1, p2, p3;
  printf("Thread Identifiers (before initialization):\n");
  printf("p1: %ld\n", p1);   // garbage value
  printf("p2: %ld\n", p2);   // why 0?
  printf("p3: %ld\n\n", p3); // why 0?

  // counter
  printf("Initial Counter : %d\n", counter); // orginally 0

  // create the threads
  Pthread_create(&p1, NULL, worker, NULL);
  Pthread_create(&p2, NULL, worker, NULL);
  Pthread_create(&p3, NULL, worker, NULL);

  // thread identifiers
  printf("\nThread Identifiers (after initialization):\n");
  printf("p1: %ld\n", p1);
  printf("p2: %ld\n", p2);
  printf("p3: %ld\n\n", p3);

  // wait for all the threads to complete
  Pthread_join(p1, NULL); // waits to terminate
  Pthread_join(p2, NULL); // waits to terminate
  Pthread_join(p3, NULL); // waits to terminate

  printf("Final value   : %d\n", counter);
  return 0;
}
