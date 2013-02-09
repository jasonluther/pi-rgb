#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

unsigned char RGB_VALUE[3] = { 0, 0, 0 };
int RGB_PIN[3] = { 4, 5, 6 };

int main(int argc, char *argv[]) {
  int i;

  if (wiringPiSetup () < 0) {
    fprintf (stderr, "wiringPiSetupGpio failed: %s\n", strerror (errno));
    return 1;
  }

  for (i = 0; i < 3; i++) {
    if (softPwmCreate(RGB_PIN[i], 0, 255) < 0) {
      fprintf (stderr, "softPwmCreate failed: %s\n", strerror (errno));
      return 1;
    }
  }

  while (1) {
    /* get the rgb values from some IPC, one byte per color */
      RGB_VALUE[0] = 255;
      RGB_VALUE[1] = 127;
      RGB_VALUE[2] = 63;
    
    for (i = 0; i < 3; i++) {
      softPwmWrite(RGB_PIN[i], RGB_VALUE[i]) ;
    }
    delay(1000);
  }

}
