#include "utils.h"
#define _USE_MATH_DEFINES
#include <math.h>


double degree_to_radian(double degree)
{
	return degree * M_PI / 180.0;
}

double calc_elapsed_time()
{
	int current_time;
	double elapsed_time;

	current_time = glutGet(GLUT_ELAPSED_TIME);
	elapsed_time = (double)(current_time - timer) / 1000.0;
	timer = current_time;

	return elapsed_time;
}

