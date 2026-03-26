// Write a program that creates two children, and connects the standard output
// of one to the standard input of the other, using the pipe() system call.

#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef int (*child_func_t)(int);

int child1(int fd) {
  printf("[C1: %d] working...\n", (int)getpid());
  printf("[C1: %d] fd => %d\n", (int)getpid(), fd);

  char buf;
  while (read(fd, &buf, 1) > 0) {
    if (write(STDOUT_FILENO, &buf, 1) != 1) {
      fprintf(stderr,
              "[C1: %d] failed to write the pipe contents to STDOUT_FILENO!\n",
              (int)getpid());
      return -1;
    }
  }
  return 0;
}

int child2(int fd) {
  printf("[C2: %d] working...\n", (int)getpid());
  printf("[C2: %d] fd => %d\n", (int)getpid(), fd);

  // write to the fd
  char *buf = "Greetings from the firstborn!\n";
  int buf_len = strlen(buf);

  int write_bytes = write(fd, buf, buf_len);
  printf("bytes written: %d\n", write_bytes);
  if (write_bytes <= 0) {
    fprintf(stderr, "[C2: %d] failed to write the whole buffer!\n",
            (int)getpid());
    return -1;
  }

  // close the pipe end
  if (close(fd) == -1) {
    fprintf(stderr, "[C2: %d] failed to close write end of the pipe!\n",
            (int)getpid());
    return -1;
  }

  return 0;
}

int main(int argc, char **argv) {
  child_func_t child_func[] = {&child1, &child2};
  // in linux; pipe[0] is READ, and pipe[1] is write
  int pipefd[2];
  int cpid;

  // create the pipe; return with exit 1 if failed
  if (pipe(pipefd) == -1) {
    fprintf(stderr, "[ERR] failed to create a pipe!\n");
    return 1;
  }

  for (int i = 0; i < 2; ++i) {
    // fork for child process
    cpid = fork();
    if (cpid < 0) {
      fprintf(stderr, "[ERR] failed to fork!\n");
      return 1;
    } else if (cpid == 0) {
      // call the appropriate child func
      if ((*child_func[i])(pipefd[i]) < 0) {
        fprintf(stderr, "[ERR] failed child%d!\n", i);
        return 1;
      }
      break;
    }
  }
  return 0;
}
