#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

unsigned char RGB_VALUE[3] = { 0, 0, 0 };
int RGB_PIN[3] = { 4, 5, 6 };
int time_to_update = 0;

void update_rgb(void) {
  int i, fd, bytes_read;
  unsigned char color[3]; /* 3 bytes */
  if ((fd = open("/var/rgbcolor", O_RDONLY)) < 0) {
    fprintf(stderr, "Unable to open /var/rgbcolor: %s\n", strerror(errno));
  }
  if ((bytes_read = read(fd, color, 3)) < 3) {
    fprintf(stderr, "Read %d bytes: %s\n", bytes_read, strerror(errno));
  }
  for (i = 0; i < 3; i++) {
    RGB_VALUE[i] = color[i];
    fprintf(stdout, "%d: %x\n", i, RGB_VALUE[i]);
  }
  if (close(fd) < 0) {
    fprintf(stderr, "Unable to close /var/rgbcolor: %s\n", strerror(errno));
  }
}

int main(int argc, char *argv[]) {
  int i;

  if (wiringPiSetup () < 0) {
    fprintf (stderr, "wiringPiSetupGpio failed: %s\n", strerror(errno));
    return 1;
  }

  for (i = 0; i < 3; i++) {
    if (softPwmCreate(RGB_PIN[i], 0, 255) < 0) {
      fprintf (stderr, "softPwmCreate failed: %s\n", strerror(errno));
      return 1;
    }
  }

  while (1) {
    update_rgb();
    
    for (i = 0; i < 3; i++) {
      fprintf(stdout, "softPwmWrite %d: %d\n", RGB_PIN[i], RGB_VALUE[i]);
      softPwmWrite(RGB_PIN[i], RGB_VALUE[i]) ;
    }
    delay(1000);
  }

}

