#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include "include/header.h"

#include "opticflow/opticflow_module.h"
#include "opticflow/opticflow_calculator.h"


int main() {
	opticflow_module_init();
	opticflow_module_start();

	while (1) {
		opticflow_module_run();
		usleep(1);
	}

	opticflow_module_stop();
	return 0;
}




