#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "rgbcmd.h"

rgbcmd_t *open_rgbcmd(int going_to_write) {
  rgbcmd_t *cmd;
  int fd, result;
  int openargs, mmapflags;
  if (going_to_write) {
    openargs = O_CREAT|O_TRUNC|O_RDWR;
    mmapflags = PROT_READ|PROT_WRITE;
  } else {
    openargs = O_RDONLY;
    mmapflags = PROT_READ;
  }
  if ((fd = open("/var/rgbcmd", openargs)) < 0) {
    fprintf(stderr, "Unable to open /var/rgbcmd: %s\n", strerror(errno));
    exit(errno);
  }
  if (going_to_write) {
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
  }
  cmd = mmap(NULL, sizeof(rgbcmd_t), mmapflags, MAP_SHARED|MAP_FILE, fd, 0);
  if (cmd == MAP_FAILED) {
    fprintf(stderr, "Unable to mmap /var/rgbcmd: %s\n", strerror(errno));
    exit(errno);
  }
  close(fd);
  return cmd;
}

