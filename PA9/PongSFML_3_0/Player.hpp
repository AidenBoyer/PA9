#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Player : public sf::Sprite {
public:
    Player(const sf::Texture& texture, float speed = 240.f)
        : sf::Sprite(texture), playerSpeed(speed)
    {
        sf::FloatRect b = getLocalBounds();
        // setOrigin(b.width * 0.5f, b.height * 0.5f);
    }

    // Call this every frame, passing the frame's delta time
    void update(float dt) {
        sf::Vector2f movement(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            movement.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            movement.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            movement.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            movement.x += 1.f;

        // Normalize movement vector for diagonal movement
        if (movement.x != 0.f || movement.y != 0.f) {
            float len = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            movement /= len;
        }

        move(movement * playerSpeed * dt);
    }

private:
    float playerSpeed = 240.f;
};