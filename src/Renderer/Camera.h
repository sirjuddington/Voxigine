
#ifndef __CAMERA_H__
#define __CAMERA_H__

class Camera
{
private:
	fpoint3_t	_position;
	fpoint3_t	_direction;
	fpoint3_t	_up;
	fpoint3_t	_strafe;
	float		_z_near;
	float		_z_far;
	float		_gl_matrix[16];

public:
	Camera();
	~Camera() {}

	fpoint3_t	getPosition() { return _position; }
	fpoint3_t	getStrafe() { return _strafe; }

	float*	getGLMatrix();
	void	applyView();

	void	set(fpoint3_t position, fpoint3_t direction);
	void	setPosition(fpoint3_t position);

	void	move(float distance);
	void	turn(float angle);
	void	moveUp(float distance);
	void	strafe(float distance);
	void	pitch(float amount);
	void	updateVectors();
};

#endif//__CAMERA_H__
