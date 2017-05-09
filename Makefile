#which compiler
CC = gcc
#Options for development
CFLAGS = -g -Wall

VPATH = include:lib/encoding:lib/v4l:lib/vision:serial:opticflow


obj = main.o opticflow_module.o opticflow_calculator.o \
		 serial_init.o serial_open.o read_ppz_data.o udp_socket.o \
			v4l2.o jpeg.o rtp.o image.o lucas_kanade.o fast_rosten.o

flow:$(obj)
	$(CC) $? -o $@ -lpthread
	#$(CC) -o flow $(obj) -lpthread

main.o:opticflow_module.h header.h opticflow_calculator.h 

opticflow_module.o:v4l2.h jpeg.h rtp.h opticflow_module.h header.h opticflow_calculator.h

opticflow_calculator.o:opticflow_calculator.h image.h lucas_kanade.h fast_rosten.h

serial_init.o:serial_.h
serial_open.o:serial_.h
read_ppz_data.o:serial_.h
udp_socket.o:udp_socket.h
v4l2.o:v4l2.h
jpeg.o:jpeg.h
rtp.o:rtp.h udp_socket.h
image.o:image.h
lucas_kanade.o:lucas_kanade.h
fast_rosten.o:fast_rosten.h

.PHONY:clean
clean:
	-rm flow $(obj)
