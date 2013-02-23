# copied from wiringPi Makefile
DEBUG	= -g -O0
#DEBUG	= -O3
CC	= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lpthread -lm

SRC	=	rgb.c write_rgbcmd.c read_rgbcmd.c
OBJ	=	$(SRC:.c=.o)
BINS	=	$(SRC:.c=)

all:	$(BINS)

rgb:	rgb.o rgbcmd.o
	@echo [link]
	@$(CC) -o $@ rgb.o rgbcmd.o $(LDFLAGS) $(LDLIBS)

write_rgbcmd: write_rgbcmd.o rgbcmd.o
	@echo [link]
	@$(CC) -o $@ write_rgbcmd.o rgbcmd.o $(LDFLAGS) $(LDLIBS)

read_rgbcmd: read_rgbcmd.o rgbcmd.o
	@echo [link]
	@$(CC) -o $@ read_rgbcmd.o rgbcmd.o $(LDFLAGS) $(LDLIBS)

.c.o:
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) *~ core tags $(BINS)

tags:	$(SRC)
	@echo [ctags]
	@ctags $(SRC)

depend:
	makedepend -Y $(SRC)

# DO NOT DELETE

install: all
	sudo apt-get install daemon lighttpd
	- sudo lighttpd-enable-mod cgi
	sudo cp 10-cgi.conf /etc/lighttpd/conf-enabled/10-cgi.conf
	- sudo service lighttpd force-reload
	sudo cp rgbcmd.cgi /var/www/rgbcmd
	sudo chmod +x /var/www/rgbcmd
	sudo cp rgb /usr/local/bin/rgb
	sudo chmod +x /usr/local/bin/rgb
	@echo
	@echo
	@echo "Add this to /etc/rc.local:"
	@echo "/usr/bin/daemon -r /home/pi/pi-rgb/rgb --name=rgb"
	@echo
