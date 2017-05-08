/*
* 打开对应串口
* 做出相应的判断以使得串口正常工作
*
* open() -> 获取串口状态（判断是否啥阻塞状态 如果是返回错误信息)
		 -> 判断是否是串口终端 （n 退出)
*
*/
#include "./serial_.h"

int serial_open(int fd, char *port)
{
	/*打开终端
	-> O_NOCTTY 表示打开的是一个终端设备,程序将不会成为该终端的控制终端
	-> O_NDELAY 表示不关心DCD信号线所处的状态（即另一端是否激活)
	*/
	fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
	if(fd == FALSE) {
		fprintf(stderr, "Can't open serial :%s", port);
		return(FALSE);
	}
	//判断串口的状态是否为阻塞状态
	if(fcntl(fd, F_SETFL, 0) < 0) {
		fprintf(stderr, "fcntl failed!\n");
		return (FALSE);
	}
	else {
		printf("fcntl = %d\n", fcntl(fd, F_SETFL, 0));
	}

	//测试是否为终端设备?????????????
	if(isatty(STDIN_FILENO) == 0) {
		fprintf(stderr, "standard input is not a terminal dev\n");
		return(FALSE);
	}
	else {
		printf("isatty success!\n");
	}
	
	printf("fd->open = %d \n", fd);

	return fd;
}

