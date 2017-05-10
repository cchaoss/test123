#which compiler
CC = gcc
#Options for development
CFLAGS = -g -Wall

VPATH = include:lib/encoding:lib/v4l:lib/vision:opticflow

obj = main.o opticflow_module.o opticflow_calculator.o \
			v4l2.o jpeg.o image.o lucas_kanade.o fast_rosten.o

flow:$(obj)
	$(CC) $? -o $@ -lpthread
	@echo Done.
#$(CC) -o flow $(obj) -lpthread

main.o:opticflow_module.h header.h opticflow_calculator.h 

opticflow_module.o:v4l2.h jpeg.h opticflow_module.h header.h opticflow_calculator.h

opticflow_calculator.o:opticflow_calculator.h image.h lucas_kanade.h fast_rosten.h

v4l2.o:v4l2.h
jpeg.o:jpeg.h
image.o:image.h
lucas_kanade.o:lucas_kanade.h
fast_rosten.o:fast_rosten.h

.PHONY:clean
clean:
	-rm flow $(obj)
