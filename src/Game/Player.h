
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

class Player : public Entity
{
private:
	fpoint3_t	_direction;
	fpoint2_t	_facing;
	fpoint3_t	_strafe;
	float		_pitch;
	bool		_flying;

public:
	Player();
	~Player();

	fpoint3_t	getDirection() { return _direction; }
	fpoint3_t	getEyePosition();

	// Movement
	void	move(float distance);
	void	turn(float angle);
	void	strafe(float distance);
	void	pitch(float amount);
	void	updateVectors();
};

#endif//__PLAYER_H__
