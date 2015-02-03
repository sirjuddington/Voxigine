
#ifndef __ENTITY_H__
#define __ENTITY_H__

class Entity
{
protected:
	fpoint3_t	_position;

public:
	Entity() {}
	virtual ~Entity() {}

	fpoint3_t	getPosition() { return _position; }
};

#endif//__ENTITY_H__
