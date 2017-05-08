#ifndef __HEADER_H
#define __HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef  DEVICE
#define  DEVICE   /dev/video0
#endif 

#ifndef OPTICFLOW_DEBUG
#define OPTICFLOW_DEBUG  0
#endif

#ifndef  DEVICE_SIZE
#define  DEVICE_SIZE  320,240
#endif 

#ifndef  DEVICE_BUFFERS
#define  DEVICE_BUFFERS	15
#endif 

#ifndef SUBDEV
#define SUBDEV TRUE
#endif

#ifndef FOV_W
#define FOV_W   0.89360857702
#endif

#ifndef FOV_H
#define FOV_H	0.67020643276
#endif

#ifndef FX
#define FX		343.1211
#endif

#ifndef FY
#define FY		348.5053
#endif

#ifndef MAX_TRACK_CORNERS
#define MAX_TRACK_CORNERS	15
#endif

#ifndef WINDOW_SIZE
#define WINDOW_SIZE	10
#endif

#ifndef SUBPIXEL_FACTOR
#define SUBPIXEL_FACTOR		10
#endif

#ifndef MAX_ITERATIONS
#define MAX_ITERATIONS		10
#endif

#ifndef THRESHOLD_VEC
#define THRESHOLD_VEC		2
#endif

#ifndef FAST9_ADAPTIVE
#define FAST9_ADAPTIVE TRUE
#endif

#ifndef FAST9_THRESHOLD
#define FAST9_THRESHOLD		20
#endif

#ifndef	FAST9_MIN_DISTANCE
#define FAST9_MIN_DISTANCE	10
#endif

#ifndef VIEWVIDEO_PORT_OUT
#define VIEWVIDEO_PORT_OUT	5000
#endif

#ifndef VIEWVIDEO_HOST
#define VIEWVIDEO_HOST		192.168.3.122
#endif

#ifndef VIEWVIDEO_BROADCAST
#define VIEWVIDEO_BROADCAST TRUE
#endif 

#endif /*end header.h*/

