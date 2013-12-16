/* 
 * File:   shapes.h
 * Author: mark
 *
 * Created on 14 December 2013, 22:21
 */

#include "vector.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#ifndef SHAPES_H
#define	SHAPES_H

void strut(const VECTOR z1, const VECTOR z2, const VECTOR x1h, const VECTOR y1h,
	const VECTOR x2h, const VECTOR y2h);

void ellipse_normal_tangent(const VECTOR c, const VECTOR u, const VECTOR v,
	const float theta, VECTOR * const point,
	VECTOR * const normal, VECTOR * const tangent);
VECTOR ellipse_point(const VECTOR c, const VECTOR u, const VECTOR v, 
	const float theta);
void ellipse(const VECTOR c, const VECTOR u, const VECTOR v,
	const float phi = 0, const int sectors = 60);
void cylinder(const VECTOR a, const VECTOR b, const VECTOR u, const VECTOR v,
	const float phi = 0, const int sectors = 60);

#endif	/* SHAPES_H */
