#ifndef OPTICFLOW_MODULE_H
#define OPTICFLOW_MODULE_H

extern struct opticflow_t opticflow;
extern struct opticflow_state_t opticflow_state;	// State of the drone to communicate with the opticflow
extern struct opticflow_result_t opticflow_result;  // The opticflow result

extern void opticflow_module_init(void);
extern void opticflow_module_run(void);
extern void opticflow_module_start(void);
extern void opticflow_module_stop(void);

#endif
