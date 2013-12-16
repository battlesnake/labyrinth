#include "types.h"
#include "vector.h"
#include "matrix.h"
#include <math.h>

void swap(double& a, double& b)
{
	double c = a;
	a = b;
	b = c;
}

void CONFIGURATION::solve(int i)
{
	double v = (s[i].p[1] - s[i].motor_offset)[1];
	double l = strut_arm;
	double d = strut_length;
	double sn = (v * v + l * l - d * d) / (2 * l * v);
	if (sn < -1) {
		s[i].error = sn + 1;
		s[i].motor_angle = -PI / 2;
		return;
	}
	if (sn > 1) {
		s[i].error = sn - 1;
		s[i].motor_angle = PI / 2;
		return;
	}
	s[i].error = 0;
	s[i].motor_angle = asin(sn);
	return;
}

void CONFIGURATION::solve()
{
	for (int i = 0; i < struts; i++)
		solve(i);
}

float CONFIGURATION::epsilon()
{
	float ret = 0;
	for (int i = 0; i < struts; i++)
		ret += s[i].error * s[i].error;
	return ret;
}
