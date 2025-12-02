#pragma once
#include <SFML/Graphics.hpp>



class Bullet : public sf::Sprite {
public:
	Bullet(const sf::Texture& texture, double speed = 100, double direction = 1)
		: sf::Sprite(texture), bulletSpeed(speed), bulletDirection(direction);

private:
	double bulletSpeed = 100;
	double bulletDirection = 1;




};