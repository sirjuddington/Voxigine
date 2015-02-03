
#include "Main.h"
#include "Player.h"
#include "Utilities/Math.h"

Player::Player()
{
	_facing.set(0.5f, 0.5f);
	_flying = true;
	updateVectors();
}

Player::~Player()
{
}

fpoint3_t Player::getEyePosition()
{
	return fpoint3_t(_position.x, _position.y, _position.z + 2.0f);
}

void Player::move(float distance)
{
	if (!_flying)
	{
		_position.x += _facing.x * distance;
		_position.y += _facing.y * distance;
	}
	else
	{
		_position.x += _direction.x * distance;
		_position.y += _direction.y * distance;
		_position.z += _direction.z * distance;
	}
}

void Player::turn(float angle)
{
	// Find rotated view point
	fpoint2_t cp2d(_position.x, _position.y);
	fpoint2_t nd = Math::rotatePoint(cp2d, cp2d + _facing, -angle);

	// Update facing direction
	_facing.x = nd.x - _position.x;
	_facing.y = nd.y - _position.y;

	// Update vectors
	updateVectors();
}

void Player::strafe(float distance)
{
	// Move along strafe vector
	_position.x += _strafe.x * distance;
	_position.y += _strafe.y * distance;
}

void Player::pitch(float amount)
{
	// Pitch camera
	_pitch += amount;

	// Clamp
	float rad90 = (float)PI*0.5f;
	if (_pitch > rad90)
		_pitch = rad90;
	if (_pitch < -rad90)
		_pitch = -rad90;

	// Update vectors
	updateVectors();
}

void Player::updateVectors()
{
	// Normalize direction
	_facing.normalize();

	// Calculate strafe vector
	_strafe = fpoint3_t(_facing.x, _facing.y, 0).cross(fpoint3_t(0, 0, 1));
	_strafe = _strafe.normalize();

	// Calculate 3d direction vector
	_direction = Math::rotateVector3D(fpoint3_t(_facing.x, _facing.y, 0), _strafe, _pitch);
	_direction = _direction.normalize();
}
