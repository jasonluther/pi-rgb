#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "rgbcmd.h"

const int PIN_RED = 4;
const int PIN_GREEN = 5;
const int PIN_BLUE = 6;
rgbcmd_t prev_cmd;

unsigned long NEXT_TIME_TO_BLINK;
int BLINK_STATE = 0;
#define BLINK_MS 1000;
int time_to_blink(void) {
  unsigned long now = millis();
  if (now >= NEXT_TIME_TO_BLINK) {
    NEXT_TIME_TO_BLINK = now + BLINK_MS;
    BLINK_STATE = BLINK_STATE ? 0 : 1;
  }
  return BLINK_STATE;
}

int main(int argc, char *argv[]) {
  int i;
  rgbcmd_t *cmd;
  NEXT_TIME_TO_BLINK = 0;

  /* wiringPiSetup() uses pins 0-17, wiringPiSetupGpio() uses BCM pins! */
  if (wiringPiSetup() < 0) {
    fprintf (stderr, "wiringPiSetup failed: %s\n", strerror(errno));
    return 1;
  }
  softPwmCreate(PIN_RED, 0, 255);
  softPwmCreate(PIN_GREEN, 0, 255);
  softPwmCreate(PIN_BLUE, 0, 255);

  cmd = open_rgbcmd(0);

  while (1) {
    switch(cmd->mode) {
      case RGB_MODE_OFF:
        cmd->red = cmd->green = cmd->blue = 0;
        break;
      case RGB_MODE_ON:
        break;
      case RGB_MODE_RAINBOW:
        cmd->red++;
        cmd->green++;
        cmd->blue++;
        break;
      case RGB_MODE_BLINK:
        if (time_to_blink()) {

        } else {

        }
        break;
      default:
        fprintf(stderr, "Invalid mode %d\n", cmd->mode);
        break;
    }
    softPwmWrite(PIN_RED, cmd->red) ;
    softPwmWrite(PIN_GREEN, cmd->green) ;
    softPwmWrite(PIN_BLUE, cmd->blue) ;
    delay(500);
  }

}

