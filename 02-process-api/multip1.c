#include <stdio.h>
// #include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ROW 10
#define MAX_COL 10

// let's do some kind of multi-processing
// let's fill a MATRIX
int matrix[MAX_ROW][MAX_COL];

void print_matrix() {
  for (int r = 0; r < MAX_ROW; r++) {
    for (int c = 0; c < MAX_COL; c++) {
      printf("%d ", matrix[r][c]);
    }
    printf("\n");
  }
  printf("\n");
}

void fill_matrix(int row_index) {
  printf("filling row: %d...\n", row_index);
  for (int col = 0; col < MAX_COL; col++) {
    matrix[row_index][col] = 1;
  }
  print_matrix();
}

int multi_process_fill() {
  printf("starting multi-process fill...\n");
  for (int row = 0; row < MAX_ROW; row++) {
    int rc = fork();
    if (rc < 0) {
      fprintf(stderr, "failed to fork process...\n");
      return 1;
    } else if (rc == 0) {
      // child process
      fill_matrix(row);
    } else {
      wait(NULL);
      printf("filled row: %d\n", row);
      // print_matrix();
      break;
    }
  }
  return 0;
}

int main() { return multi_process_fill(); }
