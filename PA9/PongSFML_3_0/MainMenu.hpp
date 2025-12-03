#pragma once

#include <SFML/Graphics.hpp>

class MainMenu {
public:
	MainMenu();

	void initMenu();
	bool click(sf::RenderWindow& window, sf::Event& event); // for recognizing mouse clicks
	void update(sf::RenderWindow& window); //for button hover effects
	void drawMenu(sf::RenderWindow& window);
private:
	sf::RectangleShape playButton;
	sf::RectangleShape quitButton;
	sf::Font font;
	sf::Text playText;
	sf::Text quitText;
	sf::Text controlsText;
	sf::Text title;

	void updateHover(sf::Vector2f mousePos); // helper function for hover effects
};
