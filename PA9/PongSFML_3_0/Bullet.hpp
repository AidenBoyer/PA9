#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>


class Bullet : public sf::Sprite {
public:
	Bullet(const sf::Texture& texture, double speed = 100, double damage = 10, sf::Vector2f direction = { 0.f,0.f })
		: sf::Sprite(texture), bulletSpeed(speed), bulletDamage(damage), bulletDirection(direction)
	{
		float len = std::sqrt(bulletDirection.x * bulletDirection.x + bulletDirection.y * bulletDirection.y);
		if (len > 0.f)
			bulletDirection /= len;
	}

	void update(float dt) {
		move(bulletDirection * static_cast<float>(bulletSpeed) * dt);
	}
	bool isAlive() const { return alive; }
	void destroy() { alive = false; }

	double getDamage() { return bulletDamage; }

	void setDirection(sf::Vector2f dir) {
		float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
		if (len > 0.f) bulletDirection = dir / len;
	}
private:
	double bulletSpeed = 100;
	double bulletDamage = 10;
	sf::Vector2f bulletDirection = {0.f,0.f};
	bool alive = true;




};