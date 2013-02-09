## Control an RGB LED with Raspberry Pi GPIO

### Overview

Use [wiringPi](https://projects.drogon.net/raspberry-pi/wiringpi/) to control a [common-anode RGB LED](http://www.adafruit.com/products/848) from the GPIO pins.

Each color is actually switched by an [NPN transistor](https://www.sparkfun.com/products/521) so that you can drive the LED with +5v and more current than the GPIO pins can handle. Use a resource like <http://www.kpsec.freeuk.com/trancirc.htm#ic> to calculate the resistor value. 

### wiringPi software PWM

Only one GPIO pin supports hardware PWM, so we get to use this [sweet software PWM library](https://projects.drogon.net/raspberry-pi/wiringpi/software-pwm-library/). 

Here's the code with no error handling:

    unsigned char RGB_VALUE[3] = { 0, 0, 0 }; /* The colors */
    int RGB_PIN[3] = { 4, 5, 6 }; /* GPIO 23, 24, 25 */
    
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

### Setting the color

To keep things simple, `rgb.c` will run all the time, and its only job is to manage the GPIO pins. 

I chose to use a file for IPC to set the color: `/var/rgbcolor` contains the 3 bytes that control red, green, and blue. 

