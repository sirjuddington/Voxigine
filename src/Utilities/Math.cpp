
#include "Main.h"
#include "Math.h"

/* Math::clamp
 * Clamps [val] to be between [min] and [max]
 *******************************************************************/
double Math::clamp(double val, double min, double max)
{
	if (val < min) val = min;
	if (val > max) val = max;
	return val;
}

/* Math::floor
 * Returns the integral floor of [val]
 *******************************************************************/
int Math::floor(double val)
{
	if (val >= 0)
		return (int)val;
	else
		return (int)val - 1;
}

/* Math::ceil
 * Returns the integral ceiling of [val]
 *******************************************************************/
int Math::ceil(double val)
{
	if (val > 0)
		return (int)val + 1;
	else
		return (int)val;
}

/* Math::round
 * Returns the closest integral value of [val]
 *******************************************************************/
int Math::round(double val)
{
	int ret = (int)val;
	if ((val - (double)ret) >= 0.5) ret++;
	return ret;
}

/* Math::distance
 * Returns the distance between [x1,y1] and [x2,y2]
 *******************************************************************/
double Math::distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

/* Math::distance3d
 * Returns the distance between [x1,y1,z1] and [x2,y2,z2]
 *******************************************************************/
double Math::distance3d(double x1, double y1, double z1, double x2, double y2, double z2)
{
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

/* Math::lineSide
 * Returns the side of the line from [x1,y1] to [x2,y2] that the
 * point at [x,y] lies on. Positive is front, negative is back, zero
 * is on the line
 *******************************************************************/
double Math::lineSide(double x, double y, double x1, double y1, double x2, double y2)
{
	return -((y-y1)*(x2-x1) - (x-x1)*(y2-y1));
}

/* Math::rotatePoint
 * Rotates [point] around [origin] by [angle] and returns the newly
 * rotated point
 *******************************************************************/
fpoint2_t Math::rotatePoint(fpoint2_t origin, fpoint2_t point, double angle)
{
	// Translate to origin
	double x = point.x - origin.x;
	double y = point.y - origin.y;

	// Maths yay
	double srot = sin(angle * DEG_TO_RAD);
	double crot = cos(angle * DEG_TO_RAD);
	double nx = crot * x - srot * y;
	double ny = srot * x + crot * y;

	// Return rotated point
	return fpoint2_t((float)nx + origin.x, (float)ny + origin.y);
}

/* Math::distanceRayLine
 * Rotates [vector] around [axis] by [angle] and returns the
 * resulting rotated vector
 *******************************************************************/
fpoint3_t Math::rotateVector3D(fpoint3_t vector, fpoint3_t axis, double angle)
{
	fpoint3_t rvec;

	// Calculate the sine and cosine of the angle once
	float crot = (float)cos(angle);
	float srot = (float)sin(angle);

	// Rotate x
	rvec.x  = (crot + (1 - crot) * axis.x * axis.x)				* vector.x;
	rvec.x += ((1 - crot) * axis.x * axis.y - axis.z * srot)	* vector.y;
	rvec.x += ((1 - crot) * axis.x * axis.z + axis.y * srot)	* vector.z;

	// Rotate y
	rvec.y  = ((1 - crot) * axis.x * axis.y + axis.z * srot)	* vector.x;
	rvec.y += (crot + (1 - crot) * axis.y * axis.y)				* vector.y;
	rvec.y += ((1 - crot) * axis.y * axis.z - axis.x * srot)	* vector.z;

	// Rotate z
	rvec.z  = ((1 - crot) * axis.x * axis.z - axis.y * srot)	* vector.x;
	rvec.z += ((1 - crot) * axis.y * axis.z + axis.x * srot)	* vector.y;
	rvec.z += (crot + (1 - crot) * axis.z * axis.z)				* vector.z;

	return rvec;
}

/* Math::degToRad
 * Converts [angle] from degrees to radians
 *******************************************************************/
double Math::degToRad(double angle)
{
	return angle * DEG_TO_RAD;
}

/* Math::radtoDeg
 * Converts [angle] from radians to degrees
 *******************************************************************/
double Math::radToDeg(double angle)
{
	return angle * RAD_TO_DEG;
}
