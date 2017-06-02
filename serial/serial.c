#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>    /*文件控制 */
#include <sys/stat.h>
#include <sys/types.h>

#include <termios.h> /*POSIX 终端控制*/
#include <errno.h>

#include <stdint.h> 
#include "stdbool.h"
#include "serial.h"

/*
* 打开对应串口
* 做出相应的判断以使得串口正常工作
* open() -> 获取串口状态（判断是否啥阻塞状态 如果是返回错误信息)
		 -> 判断是否是串口终端 （n 退出)
*/
int serial_open(int fd, char *port)
{
	/*打开终端
	-> O_NOCTTY 表示打开的是一个终端设备,程序将不会成为该终端的控制终端
	-> O_NDELAY 表示不关心DCD信号线所处的状态（即另一端是否激活)
	*/
	fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
	if(fd == false) {
		fprintf(stderr, "Can't open serial :%s", port);
		return false;
	}
	//判断串口的状态是否为阻塞状态
	if(fcntl(fd, F_SETFL, 0) < 0) {
		fprintf(stderr, "fcntl failed!\n");
		return (false);
	}
	else {
		printf("fcntl = %d\n", fcntl(fd, F_SETFL, 0));
	}

	//测试是否为终端设备?????????????
	if(isatty(STDIN_FILENO) == 0) {
		fprintf(stderr, "standard input is not a terminal dev\n");
		return false;
	}
	else {
		printf("isatty success!\n");
	}
	
	printf("fd->open = %d \n", fd);

	return fd;
}


int serial_init(int fd)
{
	struct termios newset;

	//获取当前终端的设置
	if(tcgetattr(fd, &newset) != 0) {
		fprintf(stderr, "tcgetattr failed\n");
		return -1;
	}

	//Ignore modem control lines
	newset.c_cflag |= CLOCAL;
	newset.c_cflag |= CREAD;
	newset.c_cflag &= ~HUPCL;
	newset.c_cflag &= ~CSIZE;	
	newset.c_cflag |= CS8;		//数据位8
	newset.c_cflag &= ~PARENB;	//奇偶校验none
	newset.c_cflag &= ~CSTOPB;  //停止位1
	//设置串口baud输入-> int cfsetispeed(struct termios *, speed_t speed);
	//			  输出-> int cfsetospeed(struct termios *, speed_t speed);

	cfsetispeed(&newset, B57600); //波特率57600
	cfsetospeed(&newset, B57600);

	//设置奇偶校验位
	newset.c_iflag &= INPCK;	//对输入字符忽略奇偶校验

	newset.c_lflag &= ~IEXTEN;
	newset.c_lflag &= ~ECHOK;
	newset.c_lflag &= ~ISIG;
	newset.c_lflag &= ~ICANON;
	newset.c_lflag &= ~ECHO;
	newset.c_lflag |= NOFLSH;


	//输出数据这只.c_oflag
	newset.c_oflag &= ~OPOST;   //采用原始数据输出
	newset.c_oflag &= ~ONLCR;   //采用原始数据输出

	newset.c_cc[VTIME] = 0;
	newset.c_cc[VMIN]  = 0;

	tcflush(fd, TCIFLUSH);

	if(tcsetattr(fd, TCSANOW, &newset) != 0) {
		fprintf(stderr, "set termios failed\n");
		return -1;
	}
	printf("serial initialize success\n");

	return 0;
}
