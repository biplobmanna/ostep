#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
// #include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // any files realted operations is passed onto the FS
  // which does the hard part of actually performing the IO
  // on our behalf. one can say that the FS is the
  // API for IO to the devices, with which we don't have
  // to communicate directly

  // syscall: open()
  int fd = open("/tmp/file", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  assert(fd >= 0);

  char buffer[20];
  sprintf(buffer, "hello world\n");

  // syscall: write()
  int rc = write(fd, buffer, strlen(buffer));
  assert(rc == (strlen(buffer)));

  // syscalls: fsync(), close()
  fsync(fd);
  close(fd);

  return 0;
}
