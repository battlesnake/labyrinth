#include "stewart-platform/types.h"
#include "stewart-platform/vector.h"

/* Example configuration */
CONFIGURATION demo_configuration()
{
	/* Initialise a platform with six struts */
	CONFIGURATION config = CONFIGURATION(6);

	/* Stewart platform specifications: MUST BE SET */
	config.strut_length = 265;
	config.strut_arm = 12.5;
	config.base_rx = 125;
	config.base_ry = 125;
	config.platform_rx = config.base_rx * 0.7;
	config.platform_ry = config.base_ry * 0.7;
	config.base[0] = VECTOR::unitX() * config.base_rx;
	config.base[1] = VECTOR::unitZ() * config.base_ry;
	config.base_polygon = CONFIGURATION::PLATFORM_SHAPE::POLYEDGE;
	config.platform[0] = VECTOR::unitX() * config.platform_rx;
	config.platform[1] = VECTOR::unitZ() * config.platform_ry;
	config.platform_polygon = CONFIGURATION::PLATFORM_SHAPE::ELLIPSE;

	/* Objective: used by solve() to calculate motor angles */
	config.platform_displacement = VECTOR::unitY() * config.strut_length *
		cos((config.base_rx + config.base_ry - config.platform_rx - config.platform_ry) * 0.25 / config.strut_length);
	config.pitch = 0;
	config.yaw = 0;
	config.roll = 0;

	/* VISUAL ONLY */
	config.strut_w = 3;
	config.strut_d = 2;
	config.wheel_thickness = 10;
	config.base_thickness = 10;
	config.platform_thickness = 10;

	/*
	 * MUST be called prior to solve, as it initialises the STRUT structures
	 * which contain information used by solve() regarding positions of the
	 * strut ends.
	 * 
	 * In future, will add options for crosslinking struts at each platform.
	 * For now, struts are equally spaced (angularly) around each platform.
	 */
	config.configure();

	/* Speaks for itself */
	return config;
}
