#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "include/header.h"
#include "serial/serial.h"
#include "opticflow/inter_thread_data.h"
#include "opticflow/opticflow_module.h"
#include "opticflow/opticflow_calculator.h"

#define Serial_Name "/dev/ttyUSB0"


uint8_t buf[6] = {0xAA,0,0,0,0,0};

int main() {

	int fd = 0;uint8_t i = 0;

	fd = serial_open(fd, Serial_Name);
	if(fd == false) 
		printf("open serial failed!Return fd=:%d\n", fd);
	else if(serial_init(fd) == false) 
		printf("serial initialize failed\n");
	
	opticflow_module_init();
	opticflow_module_start();

	while(1) {
		opticflow_module_run();
		
		if(i > 5) {
			i = 0;
			opticflow_result.flow_x = 93;
			opticflow_result.flow_y = -14;
			memcpy(&buf[1],&opticflow_result.flow_x,2);
			memcpy(&buf[3],&opticflow_result.flow_y,2);
			buf[5] = (buf[1]^buf[2]^buf[3]^buf[4])&0xFF;
		}
		printf(" buf[1]:%d\n,buf[2]:%d\n,buf[3]:%d\n,buf[4]:%d\n",buf[1],buf[2],buf[3],buf[4]);		
		write(fd, &buf[i], 1);
		i++;

		usleep(200000);
	}

	opticflow_module_stop();
	return 0;
}




