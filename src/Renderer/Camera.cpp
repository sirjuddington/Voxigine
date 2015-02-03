
#include "Main.h"
#include "Camera.h"
#include "Utilities/Math.h"
#include <SFML/OpenGL.hpp>

Camera::Camera()
{
	_position.set(0, 0, 0);
	_direction.set(0, 1, 0);
	_up.set(0, 0, 1);
	_strafe.set(1, 0, 0);
	memset(_gl_matrix, 0, sizeof(float) * 16);
}

float* Camera::getGLMatrix()
{
	_gl_matrix[0] = 1.0f;
	_gl_matrix[5] = 1.0f;
	_gl_matrix[10] = (_z_far + _z_near) / (_z_near - _z_far);
	_gl_matrix[14] = (2 * _z_far * _z_near) / (_z_near - _z_far);
	_gl_matrix[11] = -1.0f;

	return _gl_matrix;
}

void Camera::applyView()
{
	gluLookAt(_position.x, _position.y, _position.z,
		_position.x+_direction.x, _position.y+_direction.y, _position.z+_direction.z,
		_up.x, _up.y, _up.z);
}

/* MapRenderer3D::cameraSet
* Sets the camera position to [position], facing [direction]
*******************************************************************/
void Camera::set(fpoint3_t position, fpoint3_t direction)
{
	// Set camera position/direction
	_position = position;
	_direction = direction;
	//cam_pitch = 0;

	// Update camera vectors
	updateVectors();
}

/* MapRenderer3D::cameraSetPosition
* Moves the camera to [position]
*******************************************************************/
void Camera::setPosition(fpoint3_t position)
{
	_position = position;
}

/* MapRenderer3D::cameraMove
* Moves the camera the direction it is facing by [distance]. If [z]
* is false it will only be moved along x/y axes
*******************************************************************/
void Camera::move(float distance)
{
	// Move along direction vector
	_position.x += _direction.x * distance;
	_position.y += _direction.y * distance;
	_position.z += _direction.z * distance;
}

/* MapRenderer3D::cameraTurn
* Rotates the camera by [angle] around the z axis
*******************************************************************/
void Camera::turn(float angle)
{
	// Find rotated view point
	fpoint2_t cp2d(_position.x, _position.y);
	fpoint2_t cd2d(_direction.x, _direction.y);
	fpoint2_t nd = Math::rotatePoint(cp2d, cp2d + cd2d.normalized(), angle);

	// Update direction
	_direction.x = nd.x - _position.x;
	_direction.y = nd.y - _position.y;

	// Update vectors
	updateVectors();
}

/* MapRenderer3D::cameraMoveUp
* Moves the camera along the z axis by [distance]
*******************************************************************/
void Camera::moveUp(float distance)
{
	_position.z += distance;
}

/* MapRenderer3D::cameraStrafe
* Moves the camera along the strafe axis by [distance]
*******************************************************************/
void Camera::strafe(float distance)
{
	// Move along strafe vector
	_position.x += _strafe.x * distance;
	_position.y += _strafe.y * distance;
}

/* MapRenderer3D::cameraPitch
* Rotates the camera view around the strafe axis by [amount]
*******************************************************************/
void Camera::pitch(float amount)
{
	//// Pitch camera
	//pitch += amount;

	//// Clamp
	//double rad90 = PI*0.5;
	//if (pitch > rad90)
	//	pitch = rad90;
	//if (pitch < -rad90)
	//	pitch = -rad90;

	//// Update vectors
	//updateVectors();
}

/* MapRenderer3D::cameraUpdateVectors
* Updates the strafe and direction vectors for the camera
*******************************************************************/
void Camera::updateVectors()
{
	// Normalize direction
	_direction.normalize();

	// Calculate strafe vector
	_strafe = _direction.cross(fpoint3_t(0, 0, 1));
	_strafe = _strafe.normalize();

	// Calculate up vector
	_up = _strafe.cross(_direction).normalize();
}
