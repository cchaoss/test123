/*
* 初始化串口
*->设置波特率 baud 
*->数据流控制方式 硬件 or 软件 或者都不是
*->设置帧格式
*/

#include "./serial_.h"

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
	newset.c_cflag &= ~CSIZE;	//数据位屏蔽
	newset.c_cflag |= CS8;
	newset.c_cflag &= ~PARENB;	//关闭输入输出奇偶校验
	newset.c_cflag &= ~CSTOPB;  //一位停止位
	//设置串口baud输入-> int cfsetispeed(struct termios *, speed_t speed);
	//			  输出-> int cfsetospeed(struct termios *, speed_t speed);
	cfsetispeed(&newset, B115200);
	cfsetospeed(&newset, B115200);

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
