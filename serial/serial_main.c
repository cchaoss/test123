
#include "serial_.h"
#include <stdint.h>

#define WR_LEN 10

int main()
{
	
	char name[] = "/dev/ttyUSB2";
	char buf[1024];
	int s_fd = 0;
	int phi = 0;
	int theta = 0;
	int yaw = 0;
	int8_t ctrl = 0;
	int8_t sctrl = 0;
	char ch;

	memset(buf, '\0', 1024);
	s_fd = serial_open(s_fd, name);
	if(s_fd == FALSE) {
		fprintf(stderr, "open serial %s failed\n", name);
		exit(EXIT_FAILURE);
	}

	if(serial_init(s_fd) == FALSE){
		fprintf(stderr, "serial initialize failed\n");
		exit(EXIT_FAILURE);
	}

	printf("----------------------------begainning read data---------------------- \n");
	
	while(1) {
		while(read(s_fd, &ch, 1) != 1) {
			//printf("wait...\n");
		}
		//printf("success...\n");
		if (ctrl == 1) {
			buf[sctrl++] = ch;
		}
		if (ch == '$') {
			ctrl = 1;
			memset(buf, '\0', 1024);
			sctrl = 0;
		}
		else if(ch == '#') { 
		//	printf("%s", buf);
			sscanf(buf, "%c%c%d%c%c%d%c%c%d", &ch, &ch, &theta, &ch, &ch, &phi, &ch, &ch, &yaw);
			printf("phi = %d, theta = %d, yaw = %d\n", phi, theta, yaw);
			ctrl = 0;	
			sctrl = 0;
		}

#if 0
		if(ch == '$') 
			ctrl = 1;	
		else if(ch == '#')
			ctrl = 0;
		if (ctrl >= 1) {
			switch (ctrl){
				case 2:
					theta = (((int16_t)ch) << 8);
					break;
				case 3:
					theta |= (int16_t)ch;
					break;
				case 4:
					phi = (((int16_t)ch) << 8);
					break;
				case 5:
					phi |= (int16_t)ch;
					break;
				default:
					printf("phi = %d, theta = %d\n", phi, theta);
					break;
			}
			ctrl++;
		}	
#endif
	}

	close(s_fd);

	exit(EXIT_SUCCESS);
}
