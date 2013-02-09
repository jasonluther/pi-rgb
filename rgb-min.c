#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

unsigned char RGB_VALUE[3] = { 0, 0, 0 };
int RGB_PIN[3] = { 4, 5, 6 };

void update_RGB(void) {
  int fd;
  fd = open("/var/rgbcolor", O_RDONLY);
  read(fd, RGB_VALUE, 3);
  close(fd);
}

int main(int argc, char *argv[]) {
  int i;
  wiringPiSetup();
  for (i = 0; i < 3; i++) {
    softPwmCreate(RGB_PIN[i], 0, 255);
  }
  while (1) {
    update_RGB();
    for (i = 0; i < 3; i++) {
      softPwmWrite(RGB_PIN[i], RGB_VALUE[i]) ;
    }
    delay(500);
  }
}
