#include "./flow_position_contrl.h"
#include <stdio.h>

#define VISION_PHI_PGAIN 200
#define VISION_PHI_IGAIN 4 

#define VISION_THETA_PGAIN 200
#define VISION_THETA_IGAIN 4 

#define VISION_DESIRED_VX 0

#define VISION_DESIRED_VY 0

#define BoundAbs1(num, max) {if (num > max) num = max; else if(num < -max) num = -max;}

struct opticflow_stab_t opticflow_stab = {
	.phi_pgain = VISION_PHI_PGAIN,
	.phi_igain = VISION_PHI_IGAIN,
	.theta_pgain = VISION_THETA_PGAIN,
	.theta_igain = VISION_THETA_IGAIN,
	.desired_vx = VISION_DESIRED_VX,
	.desired_vy = VISION_DESIRED_VY

};
/**
 *  * Horizontal guidance mode enter resets the errors
 *   * and starts the controller.*/
void stabilization_enter(void)
{
	/* Reset the integrated errors */
	opticflow_stab.err_vx_int = 0;
	opticflow_stab.err_vy_int = 0;

	/* Set rool/pitch to 0 degrees and psi to current heading */
	opticflow_stab.cmd.phi = 0;
	opticflow_stab.cmd.theta = 0;
//	opticflow_stab.cmd.psi = stateGetNedToBodyEulers_i()->psi;

}
void stabilization_opticflow_update(struct opticflow_result_t *result)
{
	/* Check if we are in the correct AP_MODE before setting commands */

	float err_vx = 0;
	float err_vy = 0;
	if (result->tracked_cnt > 0) {
		err_vx = opticflow_stab.desired_vx - result->vel_x;
		err_vy = opticflow_stab.desired_vy - result->vel_y;

	}

	/* Calculate the integrated errors (TODO: bound??) */
	opticflow_stab.err_vx_int += err_vx / 100;
	opticflow_stab.err_vy_int += err_vy / 100;
	
	BoundAbs1(opticflow_stab.err_vx_int, 500);
	BoundAbs1(opticflow_stab.err_vy_int, 500);

	/* Calculate the commands */
	opticflow_stab.cmd.phi   = opticflow_stab.phi_pgain * err_vx / 100
		+ opticflow_stab.phi_igain * opticflow_stab.err_vx_int;
	opticflow_stab.cmd.theta = -(opticflow_stab.theta_pgain * err_vy / 100
			+ opticflow_stab.theta_igain * opticflow_stab.err_vy_int);

	/* Bound the roll and pitch commands */
	BoundAbs1(opticflow_stab.cmd.phi, CMD_OF_SAT);
	BoundAbs1(opticflow_stab.cmd.theta, CMD_OF_SAT);
	//printf("cmd.phi = %d, cmd.thete = %d\n", opticflow_stab.cmd.phi, opticflow_stab.cmd.theta);
}
