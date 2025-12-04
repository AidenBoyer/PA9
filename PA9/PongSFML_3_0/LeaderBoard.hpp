#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <array>
#include <memory>
#include "LeaderboardManager.hpp"

enum class LeaderboardState {
	InputName,
	Displayed
};

class Leaderboard {
public:
	Leaderboard(const std::string& firebaseUrl);

	void fetchScores();
	void draw(sf::RenderWindow& window);
	bool handleEvents(sf::RenderWindow& window, sf::Event& event);
	void update(sf::RenderWindow& window);
	void reset();

	bool isHighScore(int waves);

	void inputName(int waves);
	
	bool isDone() const { return userDone; }
private:
	LeaderboardManager manager;
	LeaderboardManager::Score topScores[5];

	sf::Font font;

	sf::Text title;
	std::array<std::unique_ptr<sf::Text>, 5> scoreTexts;
	sf::RectangleShape background;
	sf::RectangleShape continueButton;
	sf::Text continueText;
	sf::Text loadingText;

	sf::Text promptName;
	sf::Text displayName;
	sf::Text instructionText;
	sf::Text waveText;
	sf::RectangleShape inputBox;

	LeaderboardState state;
	bool userDone;
	bool dataLoaded;

	std::string playerName;
	int currentWaves;

	void initUI();
	void updateDisplay();
	void updateHover(sf::Vector2f mousePos);
	void handleNameInput(sf::Event& event);
	void submitScore();
};