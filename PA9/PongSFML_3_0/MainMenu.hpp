#pragma once

#include <SFML/Graphics.hpp>

class MainMenu {
public:
	MainMenu();

	void initMenu();
	bool click(sf::RenderWindow& window, sf::Event& event);
	void drawMenu(sf::RenderWindow& window);
private:
	sf::RectangleShape playButton;
	sf::RectangleShape quitButton;
	sf::Font font;
	sf::Text playText;
	sf::Text quitText;
	sf::Text controlsText;
	sf::Text title;
};
