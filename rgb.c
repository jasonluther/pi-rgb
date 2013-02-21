#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

unsigned char RGB_VALUE[3] = { 0, 0, 0 };
unsigned char RGB_PREV_VALUE[3] = { 0, 0, 0 };
int RGB_PIN[3] = { 4, 5, 6 };

#define MODE_OFF 0
#define MODE_ON 1
#define MODE_RAINBOW 2
#define MODE_BLINK 3
int MODE = MODE_ON; 

unsigned long NEXT_TIME_TO_BLINK;
int BLINK_STATE = 0;
#define BLINK_MS 1000;
int time_to_blink(void) {
  unsigned long now = millis();
fprintf(stderr, "time_to_blink %lu\n", now);
fprintf(stderr, "Blink: %d\n", BLINK_STATE);
fprintf(stderr, "NEXT: %lu\n", NEXT_TIME_TO_BLINK);
  if (now >= NEXT_TIME_TO_BLINK) {
    NEXT_TIME_TO_BLINK = now + BLINK_MS;
    BLINK_STATE = BLINK_STATE ? 0 : 1;
fprintf(stderr, "Blink change: %d\n", BLINK_STATE);
  }
  return BLINK_STATE;
}

int update_MODE(void) {
  int fd, bytes_read;
  if ((fd = open("/var/rgbmode", O_RDONLY)) < 0) {
    fprintf(stderr, "Unable to open /var/rgbmode: %s\n", strerror(errno));
    exit(errno);
  }
  if ((bytes_read = read(fd, &MODE, 1)) < 1) {
    fprintf(stderr, "Read %d bytes, exected 1: %s\n", bytes_read, strerror(errno));
    MODE = 0;
  }
  if (close(fd) < 0) {
    fprintf(stderr, "Unable to close /var/rgbmode: %s\n", strerror(errno));
    exit(errno);
  }
fprintf(stderr, "Mode: %d\n", MODE);
  return MODE;
}

void update_RGB(void) {
  int i, fd, bytes_read;
  unsigned char color[3]; /* 3 bytes */
  if ((fd = open("/var/rgbcolor", O_RDONLY)) < 0) {
    fprintf(stderr, "Unable to open /var/rgbcolor: %s\n", strerror(errno));
    exit(errno);
  }
  if ((bytes_read = read(fd, color, 3)) < 3) {
    fprintf(stderr, "Read %d bytes: %s\n", bytes_read, strerror(errno));
    exit(errno);
  }
  for (i = 0; i < 3; i++) {
    RGB_VALUE[i] = color[i];
    /* fprintf(stderr, "%d: %x\n", i, RGB_VALUE[i]); */
  }
  if (close(fd) < 0) {
    fprintf(stderr, "Unable to close /var/rgbcolor: %s\n", strerror(errno));
    exit(errno);
  }
}

int main(int argc, char *argv[]) {
  int i;
  NEXT_TIME_TO_BLINK = 0;

  /* wiringPiSetup() uses pins 0-17, wiringPiSetupGpio() uses BCM pins! */
  if (wiringPiSetup() < 0) {
    fprintf (stderr, "wiringPiSetup failed: %s\n", strerror(errno));
    return 1;
  }
  for (i = 0; i < 3; i++) {
    if (softPwmCreate(RGB_PIN[i], 0, 255) < 0) {
      fprintf (stderr, "softPwmCreate failed: %s\n", strerror(errno));
      return 1;
    }
  }

  while (1) {
    update_MODE();
    switch(MODE) {
      case MODE_OFF:
        for (i = 0; i < 3; i++) {
          RGB_VALUE[i] = 0;
        }
        break;
      case MODE_ON:
        update_RGB();
        break;
      case MODE_RAINBOW:
        for (i = 0; i < 3; i++) {
          RGB_VALUE[i]++;
        }
        break;
      case MODE_BLINK:
        if (time_to_blink()) {
          for (i = 0; i < 3; i++) {
            RGB_VALUE[i] = 0x00;
          }
        } else {
          update_RGB();
        }
        break;
      default:
        fprintf(stderr, "Invalid mode %d\n", MODE);
        break;
    }
    for (i = 0; i < 3; i++) {
      if (RGB_PREV_VALUE[i] != RGB_VALUE[i]) {
        fprintf(stderr, "softPwmWrite %d: %d\n", RGB_PIN[i], RGB_VALUE[i]);
        softPwmWrite(RGB_PIN[i], RGB_VALUE[i]) ;
        RGB_PREV_VALUE[i] = RGB_VALUE[i];
      }
    }
    delay(500);
  }

}

