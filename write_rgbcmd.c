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
  cmd->mode = RGB_MODE_ON;
  cmd->red = atoi(argv[1]);
  cmd->green = atoi(argv[2]);
  cmd->blue = atoi(argv[3]);
  return 0;
}
