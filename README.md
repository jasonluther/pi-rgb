## Control an RGB LED with Raspberry Pi GPIO

### Overview

Use [wiringPi](https://projects.drogon.net/raspberry-pi/wiringpi/) to control a [common-anode RGB LED](http://www.adafruit.com/products/848) from the GPIO pins.

Each color is actually switched by an [NPN transistor](https://www.sparkfun.com/products/521) so that you can drive the LED with +5v and more current than the GPIO pins can handle. Use a resource like <http://www.kpsec.freeuk.com/trancirc.htm#ic> to calculate the resistor value. 

### wiringPi software PWM

Only one GPIO pin supports hardware PWM, so we get to use this [sweet software PWM library](https://projects.drogon.net/raspberry-pi/wiringpi/software-pwm-library/). 

### Setting the color

`rgb.c` will run all the time. Its job is to manage the GPIO pins based on 
the contents of a command structure that's set using mmap() for IPC.

To set the command (color and mode), see `write_rgbcmd.c`. It uses mmap() to
choose the mode (on, off, blink, rainbow, and random) and the color for the 
on/blink modes. 

