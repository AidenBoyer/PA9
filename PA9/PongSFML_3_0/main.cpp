#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <iostream>
#include <filesystem>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "Space Defender");
    
    const float aspectRatio = 16.0f / 9.0f;

    sf::Texture playerTexture;
    std::cout << std::filesystem::current_path() << std::endl;
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

            if (const auto resized = event->getIf<sf::Event::Resized>()) {
                unsigned int width = resized->size.x;
                unsigned int height = resized->size.y;

                unsigned int newHeight = static_cast<int>(width / aspectRatio);

                window.setSize({width, newHeight});

                window.setView(sf::View(sf::FloatRect({ 0.0f, 0.0f }, { static_cast<float>(width),  static_cast<float>(newHeight) })));
            }
        }

        float dt = clock.restart().asSeconds();
        player.update(dt);


        window.clear();
        window.draw(player);
        window.display();
    }
}