# copied from wiringPi Makefile
DEBUG	= -g -O0
#DEBUG	= -O3
CC	= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lpthread -lm

SRC	=	rgb.c write_rgbcmd.c read_rgbcmd.c rgbcmd-cgi.c
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

rgbcmd-cgi: rgbcmd.o rgbcmd-cgi.o
	@echo [link]
	@$(CC) -o $@ rgbcmd-cgi.o rgbcmd.o $(LDFLAGS) $(LDLIBS)

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

copy-files: all
	sudo cp 10-cgi.conf /etc/lighttpd/conf-enabled/10-cgi.conf
	sudo cp rgbcmd-cgi /var/www/rgbcmd
	sudo chmod a+rwx /var/www/rgbcmd
	sudo cp rgb.html /var/www/index.html
	sudo install -v -m 744 -o root -g root rgb /usr/local/bin/rgb
	sudo install -v -m 744 -o root -g root rgb.upstart /etc/init.d/rgb

deps: 
	sudo apt-get install daemon lighttpd
	- sudo lighttpd-enable-mod cgi

install: deps all copy-files
	- sudo service lighttpd force-reload
	sudo touch /var/rgbcmd; sudo chown pi /var/rgbcmd
	sudo stop rgb; sudo start rgb
