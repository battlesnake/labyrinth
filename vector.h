/* 
 * File:   vector.h
 * Author: mark
 *
 * Created on 14 December 2013, 22:19
 */

#ifndef VECTOR_H
#define	VECTOR_H

#include <math.h>
#include <GL/gl.h>

class VECTOR {
private:
	GLfloat c[4] = { 0, 0, 0, 0 };
public:

	GLfloat& operator [] (const int index);
	GLfloat& operator () (const int index);

	static VECTOR zero();
	static VECTOR unitX();
	static VECTOR unitY();
	static VECTOR unitZ();

	void assign(const GLfloat x, const GLfloat y, const GLfloat z);
	void assign(const VECTOR& from);
	void assign(const VECTOR * const from);

	VECTOR(const VECTOR& from);
	VECTOR(const VECTOR * const from);
	VECTOR();
	VECTOR(const GLfloat x, const GLfloat y, const GLfloat z);

	VECTOR& operator +=(const VECTOR& operand);
	VECTOR& operator -=(const VECTOR& operand);
	VECTOR& operator *=(const GLfloat factor);
	VECTOR& operator /=(const GLfloat factor);

	GLfloat dot(const VECTOR& operand) const;
	GLfloat dotSelf() const;

	GLfloat length() const;

	GLfloat normalise();
	GLfloat normalise(const GLfloat length);

	VECTOR asUnit() const;
	VECTOR asUnit(const GLfloat length) const;

	operator const GLfloat*() const;
	VECTOR operator +(const VECTOR& operand) const;
	VECTOR operator -(const VECTOR& operand) const;
	GLfloat operator |(const VECTOR& operand) const;
	VECTOR operator *(const VECTOR& operand) const;
	VECTOR operator *(GLfloat factor) const;
	VECTOR operator /(GLfloat factor) const;
	VECTOR operator +() const;
	VECTOR operator -() const;
	VECTOR operator =(const VECTOR& source);

	GLfloat* quat(const GLfloat w);
	VECTOR hadamard(const VECTOR& operand);
};

VECTOR operator *(const GLfloat factor, const VECTOR& operand);

#endif	/* VECTOR_H */

