#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "rgbcmd.h"

int main(int argc, char *argv[]) {
  rgbcmd_t *cmd;
  cmd = open_rgbcmd(0);
  while(1) {
    fprintf(stdout, "mode = %d \t", cmd->mode);
    fprintf(stdout, "red = %d \t", cmd->red);
    fprintf(stdout, "green = %d \t", cmd->green);
    fprintf(stdout, "blue = %d \t", cmd->blue);
    fprintf(stdout, "\n");
    sleep(1);
  }
}
