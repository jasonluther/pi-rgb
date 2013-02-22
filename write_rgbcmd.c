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
  if ((fd = open("/var/rgbcmd", O_CREAT|O_TRUNC|O_RDWR)) < 0) {
    fprintf(stderr, "Unable to open /var/rgbcmd: %s\n", strerror(errno));
    exit(errno);
  }
  if ((result = lseek(fd, sizeof(rgbcmd_t), SEEK_SET)) < 0) {
    close(fd);
    fprintf(stderr, "Unable to lseek /var/rgbcmd: %s\n", strerror(errno));
    exit(errno);
  }
  if ((result = write(fd, "", 1)) != 1) {
    close(fd);
    fprintf(stderr, "Unable to write to /var/rgbcmd: %s\n", strerror(errno));
    exit(errno);
  }
  cmd = mmap(NULL, sizeof(rgbcmd_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FILE, fd, 0);
  if (cmd == MAP_FAILED) {
    fprintf(stderr, "Unable to mmap /var/rgbcmd: %s\n", strerror(errno));
    exit(errno);
  }
  cmd->mode = RGB_MODE_ON;
  cmd->red = atoi(argv[1]);
  cmd->green = atoi(argv[2]);
  cmd->blue = atoi(argv[3]);

  close(fd);
}
