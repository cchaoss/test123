#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include "include/header.h"
#include "serial/serial_.h"
#include "opticflow/opticflow_module.h"
#include "opticflow/opticflow_calculator.h"
//#include "./flow_contrl/flow_position_contrl.h"



#define MAXLINE 1024

#define S_NAME "/dev/ttyUSB1"
int main()
{
	int s_fd;
	int buf[MAXLINE];
	//int16_t cmd_phi,cmd_theta; 
	//unsigned char ch;
	s_fd = opticflow_module_init(S_NAME);
	printf("s_fd = %d\n", s_fd);
	opticflow_module_start();
	while (1) {
		memset(buf, '\0', MAXLINE);
		opticflow_module_run(s_fd);
		printf("phi = %f , theta = %f\n", opticflow_state.phi, opticflow_state.theta);
/*
		cmd_phi   = opticflow_stab.cmd.phi;
		cmd_theta = opticflow_stab.cmd.theta;

		if (write(s_fd, "$", 1) != 1)
			printf("write error1");
		ch = (unsigned char)(cmd_phi >> 8);
		if (write(s_fd, &ch, 1) != 1)
			printf("write error2");
		ch = (unsigned char)(cmd_phi );
		if (write(s_fd, &ch, 1) != 1)
			printf("write error3");
		ch = (unsigned char)(cmd_theta >> 8);
		if (write(s_fd, &ch, 1) != 1)
			printf("write error4");
		ch = (unsigned char)(cmd_theta);
		if (write(s_fd, &ch, 1) != 1)
			printf("write error5");
		if (write(s_fd, "#", 1) != 1)
			printf("write error6");
*/
		usleep(1);
	}
	opticflow_module_stop();

	return 0;
}
