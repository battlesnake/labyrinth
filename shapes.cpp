#include "shapes.h"
#include "vector.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

const double PI = 3.141592653589793238462;

void strut(const VECTOR z1, const VECTOR z2, const VECTOR x1h, const VECTOR y1h, const VECTOR x2h, const VECTOR y2h)
{
	static const int quads[] = {
		1, 0, 3, 2,
		4, 5, 6, 7,
		0, 1, 5, 4,
		1, 2, 6, 5,
		2, 3, 7, 6,
		3, 0, 4, 7

	};
	static const int coords[] = {
		0, 1, 3, 2, 4, 5, 7, 6
	};
	VECTOR pts[8];
	for (int i = 0; i < 8; i++) {
		int c = coords[i];
		if (c & 4)
			pts[i] = (c & 1 ? +x2h : -x2h) + (c & 2 ? +y2h : -y2h) + z2;
		else
			pts[i] = (c & 1 ? +x1h : -x1h) + (c & 2 ? +y1h : -y1h) + z1;
	}
	glBegin(GL_QUADS);
	for (int i = 0; i < 24; i += 4) {
		const int* quad = quads + i;
		for (int j = 0; j < 4; j++) {
			VECTOR p = pts[quad[j]];
			VECTOR u = pts[quad[(j + 3) % 4]] - p;
			VECTOR v = pts[quad[(j + 1) % 4]] - p;
			VECTOR n = (u * v).asUnit();
			glNormal3fv(n);
			glVertex3fv(p);
		}
	}
	glEnd();
}

void ellipse_normal_tangent(const VECTOR c, const VECTOR u, const VECTOR v, const float theta, VECTOR * const point, VECTOR * const normal, VECTOR * const tangent)
{
	/* Major and minor axes */
	float a = u.length(), b = v.length();
	/* Trig */
	float sn = sin(theta), cs = cos(theta);
	/* Unit basis */
	VECTOR x = u.asUnit(), y = v.asUnit();
	/* XYZ Coordinates of point */
	VECTOR p = c + (cs * u) + (sn * v);
	if (point)
		point->assign(p);
	VECTOR t = ((u.asUnit(b) * -sn) + (v.asUnit(a) * cs)).asUnit();
	if (tangent)
		tangent->assign(t);
	VECTOR n = (t * (u * v)).asUnit();
	if (normal)
		normal->assign(n);
}

VECTOR ellipse_point(const VECTOR c, const VECTOR u, const VECTOR v, const float theta)
{
	float cs, sn;
	sincosf(theta, &sn, &cs);
	return cs * u + sn * v + c;
}

void ellipse(const VECTOR c, const VECTOR u, const VECTOR v,
	const float phi, const int sectors)
{
	glBegin(GL_TRIANGLE_FAN);
	VECTOR normal = (u * v).asUnit();
	glNormal3fv(normal);
	glVertex3fv(c);
	for (int sector = 0; sector <= sectors; sector++) {
		float theta = sector * 2 * PI / sectors + phi;
		glVertex3fv(ellipse_point(c, u, v, theta));
	}
	glEnd();
}

void cylinder(const VECTOR a, const VECTOR b, const VECTOR u, const VECTOR v,
	const float phi, const int sectors)
{
	ellipse(a, u, v, phi, sectors);
	ellipse(b, u, -v, phi, sectors);
	VECTOR axis = (b - a).asUnit();
	VECTOR pts[(sectors + 1) * 2];
	for (int sector = 0; sector < sectors; sector++) {
		float theta = sector * 2 * PI / sectors + phi;
		VECTOR p = ellipse_point(VECTOR::zero(), u, v, theta);
		pts[sector * 2] = a + p;
		pts[sector * 2 + 1] = b + p;
	}
	pts[2 * sectors] = pts[0];
	pts[2 * sectors + 1] = pts[1];
	VECTOR normals[sectors + 1];
	for (int sector = 0; sector < sectors; sector++) {
		normals[sector] =
			(pts[((sector + 1) * 2) % (sectors * 2)] -
			pts[((sector + sectors - 1) * 2) % (sectors * 2)]).asUnit()
			* axis;
	}
	normals[sectors] = normals[0];
	glBegin(GL_TRIANGLE_STRIP);
	for (int sector = 0; sector <= sectors; sector++) {
		glNormal3fv(normals[sector]);
		glVertex3fv(pts[sector * 2]);
		glVertex3fv(pts[sector * 2 + 1]);
	}
	glEnd();
}
