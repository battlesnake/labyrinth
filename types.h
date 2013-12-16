/* 
 * File:   types.h
 * Author: mark
 *
 * Created on 16 December 2013, 01:40
 */

#ifndef TYPES_H
#define	TYPES_H

#include "vector.h"
#include "matrix.h"

const double PI = 3.141592653589793238462;

/*
 * Lengths are dimensionless, so I arbitrarily choose to use values in
 * millimetres
 */

/* A strut */
class STRUT {
public:

	STRUT()
	{
	}

	/* Offset of motor from centre */
	VECTOR motor_offset;

	/* Angle of motor arm from base plane */
	float motor_angle;

	/* Tangent to ellipse, at the motor_offset point */
	VECTOR tangent;

	/* Normal to ellipse, at the motor_offset point */
	VECTOR normal;

	/* 
	 * Offset of bases of strut from centres (accounting for rotary arm on
	 * base)
	 */
	VECTOR base_offset, platform_offset;

	/* 
	 * Half the minor dimensions of the strut at each end, for display
	 * purposes
	 */
	VECTOR base_halfwidth, base_halfdepth, platform_halfwidth, platform_halfdepth;

	/* Length of the strut */
	float length;

	/*
	 * Stores positions of each end of the strut, relative to centre of base
	 * platform
	 */
	VECTOR p[2];

	/* Nonzero if this strut could not be solved */
	float error;
};

/* Platform configuration objective */
class CONFIGURATION {
private:
	void setParams(const float* const x);
	void getParams(float* const x);
	void gradient(const float* const x, float * const grad, const float delta = 0.01);
public:

	CONFIGURATION(int struts)
	{
		this->struts = struts;
		s = new STRUT[struts];
	}

	~CONFIGURATION()
	{
		delete [] s;
	}

	/* pitch/yaw/roll/x/y/z must follow appear in that order in memory: */

	/* Platform rotation relative to base plane */
	float pitch = 0, yaw = 0, roll = 0;

	/* Platform centre displacement from base centre */
	VECTOR platform_displacement;

	/* Number of struts */
	int struts;

	/* Length of strut */
	float strut_length;

	/* Arm length / wheel radius between strut and motor */
	/* Wheels are displayed as ellipses to illustrate the angles better */
	float strut_arm;

	/* Smaller dimensions of strut, for display purposes only*/
	float strut_w;
	float strut_d;

	/* Wheel / arm thickness */
	float wheel_thickness;

	/* Base geometry: plane size, thickness */
	float base_rx;
	float base_ry;
	float base_thickness;

	/* Platform geometry: plane size, thickness */
	float platform_rx;
	float platform_ry;
	float platform_thickness;

	/* Ellipse, polygon with vertex attachment, polygon with edge attachment */
	enum PLATFORM_SHAPE {
		ELLIPSE, POLYVERTEX, POLYEDGE
	};

	/* Major and minor axes base */
	VECTOR base[2];

	/* Is the base a polygon?  Or an ellipse? */
	PLATFORM_SHAPE base_polygon;

	/* Major and minor axes platform */
	VECTOR platform[2];

	/* Is the platform a polygon?  Or an ellipse? */
	PLATFORM_SHAPE platform_polygon;

	/* Struts */
	STRUT* s;
	STRUT& operator [] (int index)
	{
		return this->s[index];
	}

	void configure();
	void solve();
	void solve(int strut);
	float epsilon();
	bool optimise(
		const float pitch_freedom,
		const float yaw_freedom,
		const float roll_freedom,
		const VECTOR displacement_freedom,
		const float jumpscale = 0.1,
		const int iterations = 100);
	/* yaw/pitch/roll/x/y/z */
	bool optimise(const float * const freedom,
		const float jumpscale = 0.1,
		const int iterations = 100);
};

CONFIGURATION demo_configuration();

#endif	/* TYPES_H */
