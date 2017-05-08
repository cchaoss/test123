
#which compiler
CC=gcc
#where are include files 
INCLUDE=.
#Options for development
CFLAGS = -g -Wall
#Options for release

a.out: serial_init.o serial_open.o serial_main.o
	$(CC)  $? -o $@
serial_init.o: serial_init.c serial_.h
	$(CC)  -I$(INCLUDE) $(CFLAGS) -c  serial_init.c 
serial_main.o: serial_main.c serial_.h
	$(CC)  -I$(INCLUDE) $(CFLAGS) -c  serial_main.c 
serial_open.o: serial_open.c serial_.h
	$(CC)  -I$(INCLUDE) $(CFLAGS) -c  serial_open.c 

clean:
	@rm serial_init.o serial_open.o serial_main.o
