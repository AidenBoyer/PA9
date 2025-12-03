#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include <iostream>
#include <filesystem>
#include "AudioManager.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 600, 600 }), "Space Defender");
    
    const float aspectRatio = 16.0f / 9.0f;

    AudioManager AudManager;
    AudManager.playShoot();

    sf::Texture playerTexture;
    std::cout << std::filesystem::current_path() << std::endl;
    playerTexture.loadFromFile("Ship.png");
    Player player(playerTexture);

    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("Enemy.png");
    Enemy enemy1(enemyTexture);

    sf::Clock clock;
    while (window.isOpen())
    {
        // SFML 3-style event polling returns std::optional<sf::Event>
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) 
                window.close();

            if (const auto resized = event->getIf<sf::Event::Resized>()) {
                unsigned int width = resized->size.x;
                unsigned int height = resized->size.y;

                unsigned int newHeight = static_cast<int>(width / aspectRatio);

                window.setSize({width, newHeight});

                window.setView(sf::View(sf::FloatRect({ 0.0f, 0.0f }, { static_cast<float>(width),  static_cast<float>(newHeight) })));
            }
        }

        float dt = clock.restart().asSeconds();
        player.update(dt, window);


        window.clear();
        window.draw(player);
        window.draw(enemy1);
        window.display();
    }
}