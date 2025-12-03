#pragma once
#include <SFML/Graphics.hpp>



class Player : public sf::Sprite {
public:
    Player(const sf::Texture& texture, double speed = 240)
        : sf::Sprite(texture), playerSpeed(speed)
    {
    }

    // Call this every frame, passing the frame's delta time and the window to clamp inside
    void update(float dt, const sf::RenderWindow& window) {
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

        // Clamp the player inside the window bounds
        auto bounds = getGlobalBounds();
        sf::Vector2u winSize = window.getSize();

        float minX = 0.f;
        float minY = 0.f;
        float maxX = static_cast<float>(winSize.x) - bounds.size.x;
        float maxY = static_cast<float>(winSize.y) - bounds.size.y;

        sf::Vector2f pos = getPosition();
        // Ensure position is within [min, max]
        pos.x = std::clamp(pos.x, minX, maxX);
        pos.y = std::clamp(pos.y, minY, maxY);
        setPosition(pos);
    }

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

    void destroy() { alive = false; }
    bool isAlive() const { return alive; }

private:
    float playerSpeed = 240.f;
    bool alive = true;
};