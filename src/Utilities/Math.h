
#ifndef __MATH_H__
#define __MATH_H__

#define PI	3.1415926535897932384
#define RAD_TO_DEG 57.29577951308233
#define DEG_TO_RAD 0.0174532925199433

namespace Math
{
	double		clamp(double val, double min, double max);
	int			floor(double val);
	int			ceil(double val);
	int			round(double val);
	double		distance(double x1, double y1, double x2, double y2);
	double		distance3d(double x1, double y1, double z1, double x2, double y2, double z2);
	double		lineSide(double x, double y, double x1, double y1, double x2, double y2);
	fpoint2_t	rotatePoint(fpoint2_t origin, fpoint2_t point, double angle);
	fpoint3_t	rotateVector3D(fpoint3_t vector, fpoint3_t axis, double angle);
	double		degToRad(double angle);
	double		radToDeg(double angle);
}

#endif//__MATH_H__
