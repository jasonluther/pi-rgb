#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include "rgbcmd.h"

const int PIN_RED = 4;
const int PIN_GREEN = 5;
const int PIN_BLUE = 6;
rgbcmd_t prev_cmd;

const int DELAY = 10;

unsigned long NEXT_TIME_TO_BLINK = 0;
int BLINK_STATE = 0;
const unsigned long BLINK_MS = 1000;
int time_to_blink(void) {
  unsigned long now = millis();
  if (now >= NEXT_TIME_TO_BLINK) {
    NEXT_TIME_TO_BLINK = now + BLINK_MS;
    BLINK_STATE = BLINK_STATE ? 0 : 1;
  }
  return BLINK_STATE;
}

int HUE = 0;
/* Adapted from http://www.cs.rit.edu/~ncs/color/t_convert.html */
/* but hue = 0-359, assume S & V are 1. RGB are 0-255. */
void update_rgbcmd_from_hue(int hue, rgbcmd_t *rainbow_cmd) {
  int sector;
  float fraction, q, t;
  hue %= 360;
  sector = hue / 60;
  fraction = (hue % 60) / 60.0;
//fprintf(stderr, "hue %d, sector %d, fraction %f\n", hue, sector, fraction);
  t = 255 * fraction;
  q = 255 - t;
  switch(sector) {
    case 0:
      rainbow_cmd->red = 255;
      rainbow_cmd->green = t;
      rainbow_cmd->blue = 0;
      break;
    case 1:
      rainbow_cmd->red = q;
      rainbow_cmd->green = 255;
      rainbow_cmd->blue = 0;
      break;
    case 2:
      rainbow_cmd->red = 0;
      rainbow_cmd->green = 255;
      rainbow_cmd->blue = t;
      break;
    case 3:
      rainbow_cmd->red = 0;
      rainbow_cmd->green = q;
      rainbow_cmd->blue = 255;
      break;
    case 4:
      rainbow_cmd->red = t;
      rainbow_cmd->green = 0;
      rainbow_cmd->blue = 255;
      break;
    case 5:
      rainbow_cmd->red = 255;
      rainbow_cmd->green = 0;
      rainbow_cmd->blue = q;
      break;
    default: 
      fprintf(stderr, "Did you pass in 360 or higher: %d?\n", hue);
      exit(-1); 
      break;
  }
}

void update_rgbcmd_from_random(rgbcmd_t *rainbow_cmd) {
  rainbow_cmd->red = rand() % 256;
  rainbow_cmd->green = rand() % 256;
  rainbow_cmd->blue = rand() % 256;
}

int main(int argc, char *argv[]) {
  rgbcmd_t *user_cmd, *current_cmd;
  rgbcmd_t off_cmd, rainbow_cmd;

  /* wiringPiSetup() uses pins 0-17, wiringPiSetupGpio() uses BCM pins! */
  if (wiringPiSetup() < 0) {
    fprintf (stderr, "wiringPiSetup failed: %s\n", strerror(errno));
    return 1;
  }
  softPwmCreate(PIN_RED, 0, 255);
  softPwmCreate(PIN_GREEN, 0, 255);
  softPwmCreate(PIN_BLUE, 0, 255);

  off_cmd.red = 0;
  off_cmd.green = 0;
  off_cmd.blue = 0;

  rainbow_cmd.red = 0;
  rainbow_cmd.green = 0;
  rainbow_cmd.blue = 0;

  user_cmd = open_rgbcmd(0);
  current_cmd = user_cmd;

  while (1) {
    switch(user_cmd->mode) {
      case RGB_MODE_ON:
        current_cmd = user_cmd;
        break;
      case RGB_MODE_OFF:
        current_cmd = &off_cmd;
        break;
      case RGB_MODE_RAINBOW:
        current_cmd = &rainbow_cmd;
        HUE++; if (HUE > 360) { HUE = 0; }
        update_rgbcmd_from_hue(HUE, &rainbow_cmd);
        break;
      case RGB_MODE_RANDOM:
        current_cmd = &rainbow_cmd;
        update_rgbcmd_from_random(&rainbow_cmd);
        break;
      case RGB_MODE_BLINK:
        if (time_to_blink()) {
          current_cmd = &off_cmd;
        } else {
          current_cmd = user_cmd;
        }
        break;
      default:
        fprintf(stderr, "Invalid mode %d\n", user_cmd->mode);
        current_cmd = &off_cmd;
        break;
    }
    //fprintf(stderr, "RGB %d %d %d\n", current_cmd->red, current_cmd->green, current_cmd->blue);
    softPwmWrite(PIN_RED, current_cmd->red) ;
    softPwmWrite(PIN_GREEN, current_cmd->green) ;
    softPwmWrite(PIN_BLUE, current_cmd->blue) ;
    delay(DELAY);
  }

}

