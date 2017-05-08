#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../include/header.h"
#include "../serial/serial_.h"
#include "../lib/v4l/v4l2.h"
#include "../lib/encoding/jpeg.h"
#include "../lib/encoding/rtp.h"
#include "opticflow_module.h"
#include "opticflow_calculator.h"
#include "../flow_contrl/flow_position_contrl.h"

/* The video device */
#ifndef OPTICFLOW_DEVICE
#define OPTICFLOW_DEVICE /dev/video0      ///< The video device
#endif

/* The video device size (width, height) */
#ifndef OPTICFLOW_DEVICE_SIZE
#define OPTICFLOW_DEVICE_SIZE 320,240     ///< The video device size (width, height)
#endif
#define __SIZE_HELPER(x, y) #x", "#y
#define _SIZE_HELPER(x) __SIZE_HELPER(x)

/* The video device buffers (the amount of V4L2 buffers) */
#ifndef OPTICFLOW_DEVICE_BUFFERS
#define OPTICFLOW_DEVICE_BUFFERS 4    	//oldvalue 15   ///< The video device buffers (the amount of V4L2 buffers)
#endif

/* The main opticflow variables */
struct opticflow_t opticflow;                      ///< Opticflow calculations
static struct opticflow_result_t opticflow_result; ///< The opticflow result
struct opticflow_state_t opticflow_state;   	   ///< State of the drone to communicate with the opticflow
static struct v4l2_device *opticflow_dev;          ///< The opticflow camera V4L2 device
static pthread_t opticflow_calc_thread;            ///< The optical flow calculation thread
static bool_t opticflow_got_result;                ///< When we have an optical flow calculation
static pthread_mutex_t opticflow_mutex;            ///< Mutex lock fo thread safety

/* Static functions */
static void *opticflow_module_calc(void *data);    ///< The main optical flow calculation thread

#define MEDIAN_DATASIZE 5


struct MedianFilterInt {
	float data[MEDIAN_DATASIZE], sortData[MEDIAN_DATASIZE];
	int8_t dataIndex;
};

struct MedianFilterInt median_vx;
struct MedianFilterInt median_vy;

void init_median_filter(struct MedianFilterInt *filter);
float update_median_filter(struct MedianFilterInt *filter, float new_data);

void init_median_filter(struct MedianFilterInt *filter)
{
	int i;
	for (i = 0; i < MEDIAN_DATASIZE; i++) {
		filter->data[i] = 0;
		filter->sortData[i] = 0;
	}
	filter->dataIndex = 0;

}

float update_median_filter(struct MedianFilterInt *filter, float new_data)
{
	int temp, i, j; // used to sort array

	//速度限幅 (cm/s)
	if (new_data > 1500) new_data = 1500;
	else if(new_data < -1500) new_data = -1500;

	filter->data[filter->dataIndex] = new_data;

	if (filter->dataIndex < (MEDIAN_DATASIZE - 1))
		filter->dataIndex++;
	else filter->dataIndex = 0;
	

	// Copy raw data to sort data array
	memcpy(filter->sortData, filter->data, sizeof(filter->data));

	for (i = 1; i <= (MEDIAN_DATASIZE - 1); i++) {
		temp = filter->sortData[i];
		j = i - 1;
		while (temp < filter->sortData[j] && j >= 0) {
			filter->sortData[j + 1] = filter->sortData[j];
			j = j - 1;
		}
		filter->sortData[j + 1] = temp;
	}
	return filter->sortData[(MEDIAN_DATASIZE) >> 1]; // return data value in middle of sorted array
}
/**
 * Initialize the optical flow module for the bottom camera
 */
int  opticflow_module_init(char *name)
{
	int fd;

	//初始化中值滤波数据空间
	init_median_filter(&median_vx);
	init_median_filter(&median_vy);

	// Initialize the opticflow calculation
	opticflow_calc_init(&opticflow, 320, 240);
	opticflow_got_result = FALSE;

	//Try to initialize the video device 
	opticflow_dev = v4l2_init(STRINGIFY(OPTICFLOW_DEVICE), OPTICFLOW_DEVICE_SIZE, OPTICFLOW_DEVICE_BUFFERS);
	if (opticflow_dev == NULL) 
		printf("[opticflow_module] Could not initialize the video device\n");
	
	fd = init_ppz_p(name);
	if (fd == -1) 
		return -1;
	else {
		return fd;
		printf("log: opticflow_module_init...success\n");
	}	
}

/**
 * Update the optical flow state for the calculation thread
 * and update the stabilization loops with the newest result
 */
void opticflow_module_run(int s_fd)
{
	pthread_mutex_lock(&opticflow_mutex);

	read_ppz_data(s_fd, &opticflow_state);
	if (opticflow_got_result) {
		opticflow_result.vel_x = update_median_filter(&median_vx, opticflow_result.vel_x);
		opticflow_result.vel_y = update_median_filter(&median_vy, opticflow_result.vel_y);
		//printf("vel_x       : %f             vel_y: %f\n", opticflow_result.vel_x, opticflow_result.vel_y);
		stabilization_opticflow_update(&opticflow_result);
		printf("cmd.phi = %6d, cmd.theta = %6d vx = %f , vy = %f\n", opticflow_stab.cmd.phi, opticflow_stab.cmd.theta, opticflow_result.vel_x, opticflow_result.vel_y);
		opticflow_got_result = FALSE;
	}

	pthread_mutex_unlock(&opticflow_mutex);
}

/**
 * Start the optical flow calculation
 */
void opticflow_module_start(void)
{
	// Check if we are not already running
	if (opticflow_calc_thread != 0) {
		printf("[opticflow_module] Opticflow already started!\n");
		return;
	}

	// Create the opticalflow calculation thread
	int rc = pthread_create(&opticflow_calc_thread, NULL, opticflow_module_calc, NULL);
	if (rc)
		printf("[opticflow_module] Could not initialize opticflow thread (return code: %d)\n", rc);
	printf("log: opticflow_module_start...success\n");
}

/**
 * Stop the optical flow calculation
 */
void opticflow_module_stop(void)
{
	v4l2_stop_capture(opticflow_dev);
}

/**
 * The main optical flow calculation thread
 * This thread passes the images trough the optical flow
 * calculator based on Lucas Kanade
 */
#include "errno.h"
static void *opticflow_module_calc(void *data __attribute__((unused)))
{
	// Start the streaming on the V4L2 device
	if (!v4l2_start_capture(opticflow_dev)) {
		printf("[opticflow_module] Could not start capture of the camera\n");
		return 0;
	}

	/* Main loop of the optical flow calculation */
	while (1) {
		// Try to fetch an image
		struct image_t img;
		v4l2_image_get(opticflow_dev, &img);

		// Copy the state
		pthread_mutex_lock(&opticflow_mutex);
		struct opticflow_state_t temp_state;
		memcpy(&temp_state, &opticflow_state, sizeof(struct opticflow_state_t));
		pthread_mutex_unlock(&opticflow_mutex);
		temp_state.agl = 1.0;
		// Do the optical flow calculation
		struct opticflow_result_t temp_result;
		opticflow_calc_frame(&opticflow, &temp_state, &img, &temp_result);
		//printf("-------------------------opticflow_result_t----------------------\n");
		// printf("opticflow_result_t.fps         : %f\n", temp_result.fps);
		//printf("opticflow_result_t.corner_cnt  : %d\n", temp_result.corner_cnt);
		// printf("opticflow_result_t.tracked_cnt : %d\n", temp_result.tracked_cnt);
		// printf("opticflow_result_t.flow_x      : %d\n", temp_result.flow_x);
		//printf("opticflow_result_t.flow_y      : %d\n", temp_result.flow_y);
		//printf("opticflow_result_t.flow_der_x  : %d\n", temp_result.flow_der_x);
		// printf("opticflow_result_t.flow_der_y  : %d\n", temp_result.flow_der_y);
		//printf("opticflow_result_t.vel_x       : %f\n", temp_result.vel_x);
		//printf("opticflow_result_t.vel_y       : %f\n", temp_result.vel_y);
		//printf("-------------------------opticflow_result_t----------------------\n");
		//printf("vel_x       : %f             vel_y: %f\n", temp_result.vel_x, temp_result.vel_y);
		// Copy the result if finished
		pthread_mutex_lock(&opticflow_mutex);
		memcpy(&opticflow_result, &temp_result, sizeof(struct opticflow_result_t));
		opticflow_got_result = TRUE;
		pthread_mutex_unlock(&opticflow_mutex);


		// Free the image
		v4l2_image_free(opticflow_dev, &img);
	}
}
