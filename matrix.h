/* 
 * File:   matrix.h
 * Author: mark
 *
 * Created on 15 December 2013, 20:20
 */

#ifndef MATRIX_H
#define	MATRIX_H

#include <GL/gl.h>
#include <math.h>
#include "vector.h"

class MATRIX {
private:

	GLfloat c[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

public:

	void assign(const MATRIX& from);
	void assign(const MATRIX * const from);
	void assign(
		const GLfloat m00, const GLfloat m01, const GLfloat m02, const GLfloat m03,
		const GLfloat m04, const GLfloat m05, const GLfloat m06, const GLfloat m07,
		const GLfloat m08, const GLfloat m09, const GLfloat m10, const GLfloat m11,
		const GLfloat m12, const GLfloat m13, const GLfloat m14, const GLfloat m15);
	void assign(const VECTOR x, const VECTOR y, const VECTOR z, const VECTOR w);
	void assign(const VECTOR x, const VECTOR y, const VECTOR z);

	MATRIX();
	MATRIX(bool ident);
	MATRIX(const MATRIX& from);
	MATRIX(const MATRIX * const from);
	MATRIX(const GLfloat m00, const GLfloat m01, const GLfloat m02, const GLfloat m03,
		const GLfloat m04, const GLfloat m05, const GLfloat m06, const GLfloat m07,
		const GLfloat m08, const GLfloat m09, const GLfloat m10, const GLfloat m11,
		const GLfloat m12, const GLfloat m13, const GLfloat m14, const GLfloat m15);
	MATRIX(const VECTOR x, const VECTOR y, const VECTOR z, const VECTOR w);
	MATRIX(const VECTOR x, const VECTOR y, const VECTOR z);

	GLfloat& operator () (int row, int col);
	VECTOR& operator () (int col);
	GLfloat operator () (int row, int col) const;
	VECTOR operator () (int col) const;
	GLfloat& operator [] (int idx);
	operator GLfloat const * () const;

	static MATRIX zero();
	static MATRIX identity();

	static MATRIX translation(const VECTOR x);
	static MATRIX translation(const GLfloat x, const GLfloat y, const GLfloat z);

	static MATRIX scale(const VECTOR x);
	static MATRIX scale(const GLfloat x);
	static MATRIX scale(const GLfloat x, const GLfloat y, const GLfloat z);

	static MATRIX rotationX(const GLfloat angle);
	static MATRIX rotationY(const GLfloat angle);
	static MATRIX rotationZ(const GLfloat angle);
	static MATRIX rotation(const VECTOR axis, const GLfloat angle);
	static MATRIX rotation(const GLfloat yaw, const GLfloat pitch, const GLfloat roll);

	MATRIX& operator +=(const MATRIX operand);
	MATRIX& operator -=(const MATRIX operand);
	MATRIX& operator *=(const MATRIX operand);
	MATRIX& operator *=(const GLfloat factor);
	MATRIX& operator /=(const GLfloat factor);

	MATRIX operator +() const;
	MATRIX operator -() const;
	MATRIX operator *(const MATRIX operand) const;
	VECTOR operator *(const VECTOR operand) const;
	MATRIX operator *(const GLfloat factor) const;
	MATRIX operator /(const GLfloat factor) const;

	MATRIX hadamard(const MATRIX& operand) const;
};

MATRIX operator *(const GLfloat factor, const MATRIX& operand);

#endif	/* MATRIX_H */
