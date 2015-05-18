## Control an RGB LED with Raspberry Pi GPIO

### Overview

Use [wiringPi](https://projects.drogon.net/raspberry-pi/wiringpi/) to control a [common-anode RGB LED](http://www.adafruit.com/products/848) from the GPIO pins.

git clone git://git.drogon.net/wiringPi
cd wiringPi
./build

Each color is actually switched by an [NPN transistor](https://www.sparkfun.com/products/521) so that you can drive the LED with +5v and more current than the GPIO pins can handle. Use a resource like <http://www.kpsec.freeuk.com/trancirc.htm#ic> to calculate the resistor value. 

### wiringPi software PWM

Only one GPIO pin supports hardware PWM, so we get to use this [sweet software PWM library](https://projects.drogon.net/raspberry-pi/wiringpi/software-pwm-library/). 

### Setting the color

`rgb.c` will run all the time. Its job is to manage the GPIO pins based on 
the contents of a command structure that's set using mmap() for IPC.

To set the command (color and mode), see `write_rgbcmd.c`. It uses mmap() to
choose the mode (on, off, blink, rainbow, and random) and the color for the 
on/blink modes. 

### CGI

We want to have a simple way to set the mode and color:

http://pi/rgbcmd/on/ff0012  
http://pi/rgbcmd/blink/ff0000  
http://pi/rgbcmd/off  
http://pi/rgbcmd/rainbow  
http://pi/rgbcmd/random  

### Usage

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This work is placed into the public domain.
