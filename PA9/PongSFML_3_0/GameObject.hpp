#pragma once

#include <SFML/graphics.hpp>

enum ObjectType{
	ABSTRACT,
	PLAYER,
	ENEMY,
	BULLET,
	POWER_UP
};

class GameObject {
public:
	virtual ObjectType getObjectType() const = 0;
};