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
#include <unistd.h> 
#include "stewart-platform/types.h"
#include "stewart-platform/matrix.h"
#include "stewart-platform/vector.h"
#include "stewart-platform/shapes.h"
#include "maestro/maestro.h"

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
 * For controlling a real system, you need to configure the structure before
 * and after the solve() method.  config[i].angles will give you the solver
 * result.  Remember to check config[i].error too though, or config.epsilon().
 * 
 * HINT: minimising config.epsilon() by compromising on less important
 * parameters (e.g. platform height from base) is a useful optimisation
 * objective to use when epsilon() > 0.
 * 
 * WARNING: solve() will not detect struts intersecting each other.  Excessive
 * roll will not behave as it does on screen...
 * 
 * config.optimise(freedom[], jumpscale, iterations) performs a given number of
 * iterations of weighted gradient descent, using jumpscale * freedom as the
 * regularisation vector.  The ideal values for these regularisation parameters
 * depends on the scale of your model, and the desired output precision.
 * 
 * 
 * This demo requires GL and GLUT, the solver only requires the standard library
 */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/

MAESTRO* maestro;
bool maestro_update;

void maestroReset()
{
	if (!maestro) {
		return;
	}
	for (int i = 0; i < 6; i++) {
		(*maestro)[i].setAP(0);
	}
}

void maestroDisconnect()
{
	if (!maestro) {
		return;
	}
	maestroReset();
	delete maestro;
	maestro = 0;
}

void maestroError(const char * const err)
{
	char cmd[4096];
	sprintf(cmd, "zenity --info --text=\"Maestro error:\n%s\"", err);
	system(cmd);
}

void maestroConnect()
{
	if (!maestro) {
		try {
			maestro = new MAESTRO(6);
		}
		catch (const char * const err) {
			maestroError(err);
			return;
		}
		atexit(&maestroDisconnect);
	}
}

void maestroConfigure()
{
	if (!maestro) {
		return;
	}
	try {
		for (int i = 0; i < 6; i++) {
			float pc = asin(sin(config.s[i].motor_angle)) * 50.0 / PI + 50;
			pc = (pc < 0) ? 0 : (pc > 100) ? 100 : pc;
			(*maestro)[i].setAP(pc);
		}
	}
	catch (const char * const err) {
		maestroError(err);
		return;
	}
}

/* Viewport */
const int width = 800, height = 800;
const float aspect = width * 1.0 / height;

/* Angular acceleration of yaw (left/right mouse buttons) */
float yaw_omega = 0;

/* Mouse button mask (for yaw control) */
int button = 0;

/* Run the optimiser? (middle mouse button) */
bool optimise_solution = true;

/* Rotation of entire model for display purposes ("R" key to toggle) */
bool model_rotation = true;
float model_theta = 0;

/* Target frame rate*/
const float fps_target = 60;
const float dt_target = 1.0 / fps_target;

/* Frame times for moving average rate calculation */
#define FRAME_RATE_MOVING_WINDOW_SIZE 6
clock_t t_frames[FRAME_RATE_MOVING_WINDOW_SIZE];
int t_frame = 0;

/* Extra delay to reduce frame rate */
clock_t delay_extra = 0;

void renderScene()
{
	/* Timing for animation and rate limiting */
	const clock_t ticks = clock();
	const clock_t last_ticks = t_frames[t_frame];
	t_frames[t_frame] = ticks;
	t_frame = (t_frame + 1) % FRAME_RATE_MOVING_WINDOW_SIZE;
	double dt = (ticks - last_ticks) / (1.0 * CLOCKS_PER_SEC * FRAME_RATE_MOVING_WINDOW_SIZE);

	long delayed_target = 0;
	/* Extra delay or optimisation to keep frame rate low */
	if (dt < dt_target - 1e-3) {
		delay_extra = (dt_target - dt) * CLOCKS_PER_SEC;
	}
	else if (dt < dt_target + 3e-3) {
		delay_extra *= 99;
		delay_extra /= 100;
	}
	else {
		delay_extra *= 90;
		delay_extra /= 100;
	}
	delayed_target = delay_extra + ticks;

	/* Window title */
	char title[60];

	/* Motor angles */
	config.solve();
	config.configure();
	const float error = config.epsilon();

	if (optimise_solution) {
		/* Optimise */
		const float freedoms[] = {0.05, 0.001, 0.05, 1, 1, 1};
		int opt_iterations = 0;
		const int iterations_per_cycle = 10;
		do {
			if (config.optimise(freedoms, 0.05, iterations_per_cycle))
				break;
			opt_iterations += iterations_per_cycle;
		} while (delayed_target && (clock() < delayed_target + ticks));
		const float err2 = config.epsilon();
		/* Display epsilons */
		sprintf(title, "Epsilon = %e, Optimised = %e, Optimisation cycles = %d, Frame rate = %.0f", error, err2, opt_iterations, 1 / dt);
	}
	else {
		/* Display epsilon */
		sprintf(title, "Epsilon = %e, Frame rate = %.0f", error, 1 / dt);
	}

	/* Set window title */
	glutSetWindowTitle(title);

	/* Rotation */
	if (model_rotation)
		model_theta += 45.0 * dt;

	/* Initialise buffers */
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Initialise matrices */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, -133.212, -1000);

	if (!maestro_update) {
		glRotatef(model_theta, 0, 1, 0);
	}

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
	glMultMatrixf(MATRIX::rotation(config.pitch, config.yaw, config.roll));
	cylinder(VECTOR(0, 0, 0), VECTOR(0, config.platform_thickness, 0),
		config.platform[0], config.platform[1],
		config.platform_polygon == CONFIGURATION::PLATFORM_SHAPE::POLYEDGE ? PI / config.struts : 0,
		config.platform_polygon == CONFIGURATION::PLATFORM_SHAPE::ELLIPSE ? 60 : config.struts);
	glPopMatrix();

	/* Struts and wheels */
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

	/* Flip buffers */
	glutSwapBuffers();

	/* Ensure we keep updating */
	std::async(std::launch::async, [ = ](){
		glutPostRedisplay();
	});

	/* Yaw mechanics */
	if (button & 1)
		yaw_omega += dt * 50;
	if (button & 2)
		yaw_omega -= dt * 50;
	config.yaw += yaw_omega * dt;
	yaw_omega *= pow(0.01, dt);

	/* Update maestro */
	if (maestro_update) {
		maestroConfigure();
	}

	/* Delay to reduce frame rate */
	if (delayed_target > clock())
		usleep((1000000 * (delayed_target - clock())) / CLOCKS_PER_SEC);
	while (clock() < delayed_target) {

	}

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
	/* Roll/pitch in response to cursor position */
	config.roll = PI / 4 * (x * 1.0 / width - 0.5);
	config.pitch = PI / 4 * (y * 1.0 / height - 0.5);
}

void mouseClick(int btn, int state, int x, int y)
{
	/* Smooth yaw with left/right mouse buttons */
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
	/* Middle button to temporarily disable the optimiser */
	if (btn == GLUT_MIDDLE_BUTTON)
		optimise_solution = state == GLUT_UP;
}

void keyPress(unsigned char key, int x, int y)
{
	if (key > 'Z')
		key -= 'a' - 'A';
	if (key == 'Q' || key == 27)
		exit(0);
	if (key == 'R')
		model_rotation = !model_rotation;
	if (key == 'D') {
		maestroConnect();
		maestro_update = !maestro_update;
		if (!maestro_update) {
			maestroReset();
		}
	}
}

void specialKeyPress(int key, int x, int y)
{
}

const char help[] =
	"zenity --info --no-wrap --text='"
	"Stewart platform solver/optimiser demo\n"
	"\n"
	" * Move cursor in window to control pitch and roll\n"
	"\n"
	" * Use the left/right mouse buttons to control yaw\n"
	"\n"
	" * Hold down the middle button to temporarily disable the optimiser,\n"
	"allowing you to see which struts (if any) are in impossible positions\n"
	"\n"
	" * Press 'R' to toggle the model rotation\n"
	"\n"
	" * Press 'D' to toggle sending of the configuration to a connected Maestro-driven platform\n"
	"   NOTE: The Maestro must be in \"USB\" mode in order for this feature to work\n"
	"\n"
	" * Press 'Q' to quit the demo\n"
	"'";

int main(int argc, char** argv)
{
	system(help);
	createWindow(argc, argv);
	glutDisplayFunc(&renderScene);
	glutPassiveMotionFunc(&mouseMove);
	glutMotionFunc(&mouseMove);
	glutMouseFunc(&mouseClick);
	glutKeyboardFunc(&keyPress);
	glutSpecialFunc(&specialKeyPress);
	setCamera();
	setLights();
	glutMainLoop();
	return 0;
}
