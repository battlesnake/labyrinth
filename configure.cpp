#include "types.h"
#include "shapes.h"
#include "vector.h"
#include "matrix.h"
#include <math.h>

void CONFIGURATION::configure()
{
	MATRIX platform_rotation = MATRIX::rotation(pitch, yaw, roll);
	for (int i = 0; i < struts; i++) {
		float theta = i * 2 * PI / struts;
		/* Motor */
		ellipse_normal_tangent(VECTOR::zero(), base[0], base[1], theta, &s[i].motor_offset, &s[i].normal, &s[i].tangent);
		if (base_polygon == PLATFORM_SHAPE::POLYEDGE)
			s[i].motor_offset *= cos(PI / struts);
		s[i].base_halfwidth = s[i].tangent * strut_w / 2;
		s[i].base_halfdepth = s[i].normal * strut_d / 2;
		/* Platform */
		ellipse_normal_tangent(VECTOR::zero(), platform[0], platform[1], theta, &s[i].platform_offset, &s[i].normal, &s[i].tangent);
		if (platform_polygon == PLATFORM_SHAPE::POLYEDGE)
			s[i].platform_offset *= (1 - cos(PI / struts));
		s[i].platform_halfwidth = s[i].tangent * strut_w / 2;
		s[i].platform_halfdepth = s[i].normal * strut_d / 2;
		/* Arm angle & base position */
		float cs, sn;
		sincosf(s[i].motor_angle, &sn, &cs);
		s[i].base_offset =
			s[i].motor_offset +
			cs * s[i].tangent * strut_arm +
			sn * VECTOR(0, strut_arm, 0) +
			s[i].normal * wheel_thickness;
		/* Length */
		s[i].length = strut_length;
		/* Ends */
		s[i].p[0] = s[i].base_offset;
		s[i].p[1] = platform_displacement + platform_rotation * s[i].platform_offset;
	}
}
