#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <iostream>
#include <filesystem>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "Space Defender");


    sf::Texture playerTexture;
    playerTexture.loadFromFile("Ship.png");
    Player player(playerTexture);


    sf::Clock clock;
    while (window.isOpen())
    {
        // SFML 3-style event polling returns std::optional<sf::Event>
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = clock.restart().asSeconds();
        player.update(dt);


        window.clear();
        window.draw(player);
        window.display();
    }
}