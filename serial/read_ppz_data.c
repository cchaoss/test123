
#include "./serial_.h"

#define WR_LEN 10

int init_ppz_p(char *name)
{
	int s_fd = 0;
	s_fd = serial_open(s_fd, name);
	if(s_fd == FALSE) {
		fprintf(stderr, "open serial %s failed\n", name);
		return -1;
	}
	if(serial_init(s_fd) == FALSE){
		fprintf(stderr, "serial initialize failed\n");
		return -1;
	}
	
	return s_fd;
}
int read_ppz_data(int s_fd, struct opticflow_state_t *ppz_data)
{
	
	char buf[1024];
	int phi = 0;
	int theta = 0;
	int yaw = 0;
	int8_t ctrl = 0;
	int8_t sctrl = 0;
	char ch;

//	memset(buf, '\0', 1024);
	while(1) {
		while(read(s_fd, &ch, 1) != 1) {
			//printf("wait...\n");
		}
		if (ctrl == 1){
			buf[sctrl++] = ch;
		}
		if (ch == '$') {
			ctrl = 1;
			memset(buf, '\0', 1024);
			sctrl = 0;
		}
		else if(ch == '#') { 
			sscanf(buf, "%c%c%d%c%c%d%c%c%d", &ch, &ch, &theta, &ch, &ch, &phi, &ch, &ch, &yaw);
			ppz_data->phi = phi/100.0f;
			ppz_data->theta = theta/100.0f;
			ppz_data->agl = 1;
			ctrl = 0;	
			sctrl = 0;
			break;
		}
	}
	//close(s_fd);
	return 0;
}
