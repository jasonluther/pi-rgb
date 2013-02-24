#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "rgbcmd.h"

rgbcmd_t *open_rgbcmd(int need_to_create) {
  rgbcmd_t *cmd;
  int fd, result;
  int openargs, mmapflags;
  mmapflags = PROT_READ|PROT_WRITE;
  if (need_to_create) {
    openargs = O_CREAT|O_TRUNC|O_RDWR;
  } else {
    openargs = O_RDWR;
  }
  if ((fd = open("/var/rgbcmd", openargs)) < 0) {
    fprintf(stderr, "Unable to open /var/rgbcmd: %s\n", strerror(errno));
    exit(errno);
  }
  if (need_to_create) {
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

void rgbcmd_json(rgbcmd_t *cmd, char *buffer, size_t length) {
  char *modestring;
  switch(cmd->mode) {
    case RGB_MODE_ON:
      modestring = "on";
      break;
    case RGB_MODE_OFF:
      modestring = "off";
      break;
    case RGB_MODE_BLINK:
      modestring = "blink";
      break;
    case RGB_MODE_RAINBOW:
      modestring = "rainbow";
      break;
    case RGB_MODE_RANDOM:
      modestring = "random";
      break;
    default:
      modestring = "unknown";
  }
  snprintf(buffer, length, 
    "{command:\"%s\",red:%d,green:%d,blue:%d,hex:\"%02x%02x%02x\"}", 
    modestring, 
    cmd->red, cmd->green, cmd->blue, 
    cmd->red, cmd->green, cmd->blue
  );
}

