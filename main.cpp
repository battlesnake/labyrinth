/* 
 * File:   main.cpp
 * Author: Mark K Cowan, mark@battlesnake.co.uk, hackology.co.uk
 */

#include <time.h>
#include <future>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "types.h"
#include "matrix.h"
#include "vector.h"
#include "shapes.h"

using namespace std;

/*
 * Demo configuration.  Look in types.h:CONFIGURATION and demo.cpp for details
 * regarding the parameters in the structure.
 */
CONFIGURATION config = demo_configuration();

/* Mouse events alter the yaw, pitch, roll properties of the config */

/*
 * At the start of each frame, we call:
 * 
 *   config.solve()
 *     This calculates the motor angles required to achieve the desired 
 *     angles and displacement.  It calculates errors too when the desired
 *     parameters are not possible.  This may be accessed via config[i].error
 *     for a signed error on a particular strut, or config.epsilon() for the
 *     sum of squares of those strut errors.
 * 
 *   config.configure()
 *     This uses the angles, displacement,known parameters (e.g. base radius,
 *     platform radius, strut lengths) to calculate the start and end position
 *     of each strut.
 * 
 * For controlling a real system, you only need to configure the structure then
 * hit the solve() method.  config[i].angles will give you the result.  Remember
 * to check config[i].error too though, or config.epsilon().
 * 
 * HINT: minimising config.epsilon() by compromising on less important
 * parameters (e.g. platform height from base) is a useful optimisation
 * objective to use when epsilon() > 0.
 * 
 * WARNING: solve() will not detect struts intersecting each other.  Excessive
 * roll will not behave as it does on screen...
 * 
 */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* Viewport */
const int width = 800, height = 800;
const float aspect = width * 1.0 / height;

/* Angular acceleration of pitch */
float omegapitch = 0;

/* Mouse button mask */
int button = 0;

/* Ticks when last frame was drawn */
clock_t last_ticks = clock();

void renderScene()
{
	/* Timing */
	double dt = (clock() - last_ticks) * 1.0 / CLOCKS_PER_SEC;
	last_ticks = clock();
	/* Motor angles */
	config.solve();
	config.configure();
	/* Display epsilon */
	char title[60];
	sprintf(title, "Epsilon: %e", config.epsilon());
	glutSetWindowTitle(title);

	glMatrixMode(GL_MODELVIEW);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0, -133.212, -1000);

	glRotatef(clock() * 45.0 / CLOCKS_PER_SEC, 0, 1, 0);

	glPushMatrix();
	{
		/* Base */
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, VECTOR(0.8, 0.7, 0.1));
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, VECTOR(1, 1, 1));
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 90);
		cylinder(VECTOR(0, -config.base_thickness, 0), VECTOR(0, 0, 0),
			config.base[0], config.base[1],
			config.base_polygon == CONFIGURATION::PLATFORM_SHAPE::POLYEDGE ? PI / config.struts : 0,
			config.base_polygon == CONFIGURATION::PLATFORM_SHAPE::ELLIPSE ? 60 : config.struts);
		/* Platform */
		glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, VECTOR(0.8, 0.4, 0.3));
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, VECTOR(1, 1, 1));
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 90);
		glMultMatrixf(MATRIX::translation(config.platform_displacement));
		glMultMatrixf(MATRIX::rotation(config.yaw, config.pitch, config.roll));
		cylinder(VECTOR(0, 0, 0), VECTOR(0, config.platform_thickness, 0),
			config.platform[0], config.platform[1],
			config.platform_polygon == CONFIGURATION::PLATFORM_SHAPE::POLYEDGE ? PI / config.struts : 0,
			config.platform_polygon == CONFIGURATION::PLATFORM_SHAPE::ELLIPSE ? 60 : config.struts);
		glPopMatrix();

		for (int i = 0; i < config.struts; i++) {
			/* Strut */
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, VECTOR(0.2, 0.4, 0.7));
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, VECTOR(1, 1, 1));
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 10);
			if (config[i].error != 0)
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, VECTOR(1, 0, 0));
			strut(
				config[i].p[0], config[i].p[1], 
				config[i].base_halfwidth, config[i].base_halfdepth, 
				config[i].platform_halfwidth, config[i].platform_halfdepth);
			/* Wheel */
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, VECTOR(0.2, 0.7, 0.2));
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, VECTOR(1, 1, 1));
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 80);
			MATRIX m = MATRIX::rotation(config[i].normal, -config[i].motor_angle);
			VECTOR u = m * config[i].tangent;
			VECTOR v = m * VECTOR(0, 0.5, 0);
			cylinder(config[i].motor_offset, config[i].motor_offset + config[i].normal * config.wheel_thickness, u * config.strut_arm, v * config.strut_arm);
		}

	}
	glPopMatrix();

	glutSwapBuffers();

	/* Ensure we keep updating */
	std::async(std::launch::async, [ = ](){
		glutPostRedisplay();
	});

	if (button & 1)
		omegapitch += dt * 50;
	if (button & 2)
		omegapitch -= dt * 50;
	config.pitch += omegapitch * dt;
	omegapitch *= pow(0.03, dt);
}

void setLights()
{
	glMatrixMode(GL_PROJECTION);

	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);

	/* Diffuse light coming from back right */
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, VECTOR(-0.4, -0.2, -1).quat(0));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, VECTOR(0.8, 0.8, 0.8).quat(1));
	glLightfv(GL_LIGHT0, GL_SPECULAR, VECTOR(1, 1, 1).quat(1));
	glLightfv(GL_LIGHT0, GL_AMBIENT, VECTOR(0.3, 0.3, 0.3).quat(1));

	/* Specular light between platforms */
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, VECTOR(0, 0, -1100).quat(1));
	glLightfv(GL_LIGHT1, GL_DIFFUSE, VECTOR(0.2, 0.2, 0.2).quat(1));
	glLightfv(GL_LIGHT1, GL_SPECULAR, VECTOR(1, 1, 1).quat(1));
	glLightfv(GL_LIGHT1, GL_AMBIENT, VECTOR(0, 0, 0).quat(1));

	/* Specular light from the left */
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_POSITION, VECTOR(-500, 50, -500).quat(1));
	glLightfv(GL_LIGHT2, GL_DIFFUSE, VECTOR(0.0, 0.0, 0.0).quat(1));
	glLightfv(GL_LIGHT2, GL_SPECULAR, VECTOR(0.5, 0.5, 0.5).quat(1));
	glLightfv(GL_LIGHT2, GL_AMBIENT, VECTOR(0, 0, 0).quat(1));

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void setCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-4 * aspect, 4 * aspect, -2, 6, 0, 100);
	gluPerspective(25, aspect, 1, 1000000);
}

void createWindow(int& argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Labyrinth");
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_MULTISAMPLE_ARB);
}

void mouseMove(int x, int y)
{
	config.roll = PI / 4 * (x * 1.0 / width - 0.5);
	config.yaw = PI / 4 * (y * 1.0 / height - 0.5);
}

void mouseClick(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON)
		if (state != GLUT_UP)
			button |= 1;
		else
			button &= ~1;
	if (btn == GLUT_RIGHT_BUTTON)
		if (state != GLUT_UP)
			button |= 2;
		else
			button &= ~2;
}

void keyPress(unsigned char key, int x, int y)
{
	if (key == 'Q' || key == 'q' || key == 27)
		exit(0);
}

int main(int argc, char** argv)
{
	createWindow(argc, argv);
	glutDisplayFunc(&renderScene);
	glutPassiveMotionFunc(&mouseMove);
	glutMotionFunc(&mouseMove);
	glutMouseFunc(&mouseClick);
	glutKeyboardFunc(&keyPress);
	setCamera();
	setLights();
	glutMainLoop();
	return 0;
}
