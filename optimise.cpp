#include "types.h"

void CONFIGURATION::setParams(const float* const x)
{
	float* params = &pitch;
	for (int i = 0; i < 6; i++)
		params[i] = x[i];
}

void CONFIGURATION::getParams(float* const x)
{
	float* params = &pitch;
	for (int i = 0; i < 6; i++)
		x[i] = params[i];
}

void CONFIGURATION::gradient(const float* const x, float * const grad, const float delta)
{
	float* params = &pitch;
	float old[6];
	getParams(old);
	for (int i = 0; i < 6; i++) {
		setParams(x);
		params[i] += delta / 2;
		configure();
		solve();
		grad[i] = epsilon();
	}
	for (int i = 0; i < 6; i++) {
		setParams(x);
		params[i] -= delta / 2;
		configure();
		solve();
		grad[i] -= epsilon();
	}
	setParams(old);
	for (int i = 0; i < 6; i++)
		grad[i] /= delta;
}

bool CONFIGURATION::optimise(
	const float pitch_freedom,
	const float yaw_freedom,
	const float roll_freedom,
	const VECTOR displacement_freedom,
	const float jumpscale,
	const int iterations)
{
	float freedom[] = {pitch_freedom, yaw_freedom, roll_freedom, displacement_freedom[0], displacement_freedom[1], displacement_freedom[2]};
	optimise(freedom, jumpscale, iterations);
}

bool CONFIGURATION::optimise(const float * const freedom,
	const float jumpscale,
	const int iterations)
{
	float x[6], grad[6];
	bool done = false;
	getParams(x);
	for (int i = 0; i < iterations; i++) {
		gradient(x, grad);
		for (int j = 0; j < 6; j++) {
			x[j] -= grad[j] * jumpscale * freedom[j];
		}
		if (done = (epsilon() == 0))
			break;
	}
	setParams(x);
	return done;
}
