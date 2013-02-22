#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "rgbcmd.h"

int main(int argc, char *argv[]) {
  rgbcmd_t *cmd;
  int fd, result;
  if ((fd = open("/var/rgbcmd", O_RDONLY)) < 0) {
    fprintf(stderr, "Unable to open /var/rgbcmd: %s\n", strerror(errno));
    exit(errno);
  }
  cmd = mmap(NULL, sizeof(rgbcmd_t), PROT_READ, MAP_SHARED|MAP_FILE, fd, 0);
  if (cmd == MAP_FAILED) {
    fprintf(stderr, "Unable to mmap /var/rgbcmd: %s\n", strerror(errno));
    exit(errno);
  }
  while(1) {
    fprintf(stdout, "mode = %d \t", cmd->mode);
    fprintf(stdout, "red = %d \t", cmd->red);
    fprintf(stdout, "green = %d \t", cmd->green);
    fprintf(stdout, "blue = %d \t", cmd->blue);
    fprintf(stdout, "\n");
    sleep(1);
  }
  close(fd);
}
