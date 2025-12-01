#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    //sf::Image image("Ship.png");

    // Load a texture
    sf::Texture texture;
    //texture.loadFromImage(image);
    texture.loadFromFile("Ship.png");
    

    // Create a sprite
    sf::Sprite sprite(texture);
    /*sprite.setTextureRect({ {10, 10}, {50, 30} });
    sprite.setColor({ 255, 255, 255, 200 });
    sprite.setPosition({ 100.f, 25.f });*/


    while (window.isOpen())
    {
        // SFML 3-style event polling returns std::optional<sf::Event>
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        // Draw objects before displaying
        window.draw(shape);
        window.draw(sprite);
        window.display();
    }
}