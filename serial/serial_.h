/*
*serial需要的一些头文件
*
*/

#ifndef __SERIAL_H__
#define __SERIAL_H__

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
#include "../opticflow/inter_thread_data.h" /*struct opticflow_state_t 结构*/

#ifndef FALSE
#define FALSE -1
#endif

extern int serial_init(int fd);
extern int serial_open(int fd, char *port);
extern int init_ppz_p(char *name);
extern int read_ppz_data(int s_fd, struct opticflow_state_t *ppz_data);

#endif 
