#ifndef _flow_position_contrl_H_
#define _flow_position_contrl_H_

#include <stdint.h>
#include <unistd.h>
#include "../opticflow/inter_thread_data.h"
#include "../include/std.h"


#define CMD_OF_SAT 1500
#define MAX_err_vx_int 100000
#define MAX_err_vy_int 100000

struct Int32Eulers {
  int32_t phi;   ///< in rad with #INT32_ANGLE_FRAC roll
  int32_t theta; ///< in rad with #INT32_ANGLE_FRAC pith
  int32_t psi;   ///< in rad with #INT32_ANGLE_FRAC yaw
};

struct opticflow_stab_t {
	
  int32_t phi_pgain;        ///< The roll P gain on the err_vx
  int32_t phi_igain;        ///< The roll I gain on the err_vx_int
  int32_t theta_pgain;      ///< The pitch P gain on the err_vy
  int32_t theta_igain;      ///< The pitch I gain on the err_vy_int
  float desired_vx;         ///< The desired velocity in the x direction (cm/s)
  float desired_vy;         ///< The desired velocity in the y direction (cm/s)
  
  float err_vx_int;         ///< The integrated velocity error in x direction (m/s)  ——x方向整合速度误差
  float err_vy_int;         ///< The integrated velocity error in y direction (m/s)
  struct Int32Eulers cmd;   ///< The commands that are send to the hover loop
};

extern struct opticflow_stab_t opticflow_stab;
extern void stabilization_opticflow_update(struct opticflow_result_t *result);
#endif
