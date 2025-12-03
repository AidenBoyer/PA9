#include "MainMenu.hpp"

MainMenu::MainMenu() : playText(font), quitText(font), title(font), controlsText(font) {
	font.openFromFile("Hunters K-Pop.otf");
	initMenu();
}

void MainMenu::initMenu() {
	title.setString("Galaxy Shooter");
	title.setCharacterSize(50);
	title.setFillColor(sf::Color::Yellow); //change color later
	title.setPosition(sf::Vector2f(400.f - title.getGlobalBounds().size.x / 2.f, 100.f));

	playButton.setSize(sf::Vector2f(200.f, 60.f));
	playButton.setPosition(sf::Vector2f(300.f, 250.f));
	playButton.setFillColor(sf::Color(1,1,1)); //change color later
	playButton.setOutlineThickness(2.f);
	playButton.setOutlineColor(sf::Color::White);

	playText.setString("Play Game");
	playText.setCharacterSize(25);
	playText.setFillColor(sf::Color::White);
	playText.setPosition(sf::Vector2f(350.f, 250.f));

	quitButton.setSize(sf::Vector2f(200.f, 60.f));
	quitButton.setPosition(sf::Vector2f(300.f, 350.f));
	quitButton.setFillColor(sf::Color(1, 1, 1)); //change color later
	quitButton.setOutlineThickness(2.f);
	quitButton.setOutlineColor(sf::Color::White);

	quitText.setString("Quit");
	quitText.setCharacterSize(25);
	quitText.setFillColor(sf::Color::White);
	quitText.setPosition(sf::Vector2f(350.f, 355.f));

	controlsText.setString("Controls: AD to move left and right, Spacebar to shoot");
	controlsText.setCharacterSize(15);
	controlsText.setFillColor(sf::Color::White);
	controlsText.setPosition(sf::Vector2f(400.f - controlsText.getGlobalBounds().size.x / 2.f, 550.f));
}

bool MainMenu::click(sf::RenderWindow& window, sf::Event& event) {
	if (const auto mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
			sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			if (playButton.getGlobalBounds().contains(mousePos)) {
				return true;
			}
			else if (quitButton.getGlobalBounds().contains(mousePos)) {
				window.close();
			}
		}
	}
	return false;
}

void MainMenu::drawMenu(sf::RenderWindow& window) {
	window.draw(title);
	window.draw(playButton);
	window.draw(playText);
	window.draw(quitButton);
	window.draw(quitText);
	window.draw(controlsText);
}
