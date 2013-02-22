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
  cmd = open_rgbcmd(1);
  cmd->red = atoi(argv[1]);
  cmd->green = atoi(argv[2]);
  cmd->blue = atoi(argv[3]);
  if (argc == 5) {
    cmd->mode = atoi(argv[4]);
  } else {
    cmd->mode = RGB_MODE_ON;
  }
  return 0;
}
