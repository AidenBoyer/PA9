#pragma once
#include <SFML/Graphics.hpp>

class Enemy : public sf::Sprite 
{

public:
	Enemy(const sf::Texture& texture)
		: sf::Sprite(texture)
	{

	}

	void propagateEnemy()
	{

	}

private:
	//assign HP to enemy
	int health = 50;


};
