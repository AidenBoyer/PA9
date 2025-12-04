#include "LeaderBoard.hpp"

Leaderboard::Leaderboard(const std::string& firebaseUrl) : manager(firebaseUrl), title(font), continueText(font), loadingText(font), 
promptName(font), displayName(font), instructionText(font), 
waveText(font), userDone(false), dataLoaded(false), currentWaves(0), state(LeaderboardState::Displayed) {
	if (!font.openFromFile("Hunters k-Pop.otf")) {

	}

	for (int i = 0; i < 5; ++i) {
		scoreTexts[i] = std::make_unique<sf::Text>(font);
	}

	for (int i = 0; i < 5; ++i) {
		topScores[i] = { "", 0 };
	}
	
	initUI();
}

void Leaderboard::fetchScores() {
	LeaderboardManager::Score* fetchedscores = manager.GetLeaderBoard();

	if (fetchedscores != nullptr) {
		for (int i = 0; i < 5; ++i) {
			topScores[i] = fetchedscores[i];
		}
		dataLoaded = true;
		updateDisplay();
	}
	else {
		std::cerr << "Failed to fetch leaderboard." << std::endl;
		dataLoaded = false;
	}
}

void Leaderboard::draw(sf::RenderWindow& window) {
	window.draw(background);
	
	if (state == LeaderboardState::InputName) {
		window.draw(waveText);
		window.draw(promptName);
		window.draw(inputBox);
		window.draw(displayName);
		window.draw(instructionText);
	}
	else if (state == LeaderboardState::Displayed) {
		window.draw(title);
		if (dataLoaded) {
			for (int i = 0; i < 5; ++i) {
				window.draw(*scoreTexts[i]);
			}
		}
		else {
			window.draw(loadingText);
		}
		window.draw(continueButton);
		window.draw(continueText);
	}
}

bool Leaderboard::handleEvents(sf::RenderWindow& window, sf::Event& event) {
	if (state == LeaderboardState::InputName) {
		handleNameInput(event);
		return false;
	}
	else if (state == LeaderboardState::Displayed) {
		if (const auto mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
			if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
				sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				if (continueButton.getGlobalBounds().contains(mousePos)) {
					return true;
				}
			}
		}
	}
	return false;
}

void Leaderboard::update(sf::RenderWindow& window) {
	if (state == LeaderboardState::Displayed) {
		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		updateHover(mousePos);
	}
}

void Leaderboard::reset() {
	for (int i = 0; i < 5; ++i) {
		topScores[i] = { "", 0 };
	}
	dataLoaded = false;
	userDone = false;
	playerName.clear();
	state = LeaderboardState::Displayed;
	updateDisplay();
}

bool Leaderboard::isHighScore(int waves) {
	for (int i = 0; i < 5; ++i) {
		if (topScores[i].Name.empty() || waves >topScores[i].Waves) {
			return true;
		}
	}
	return false;
}

void Leaderboard::inputName(int waves) {
	currentWaves = waves;
	playerName.clear();
	state = LeaderboardState::InputName;
	userDone = false;

	waveText.setString("Waves Survived: " + std::to_string(waves));
	displayName.setString("");
}

void Leaderboard::initUI() {
	background.setSize(sf::Vector2f( 600.0f, 400.0f ));
	background.setPosition(sf::Vector2f(100.0f, 25.0f ));
	background.setFillColor(sf::Color(20, 20, 40, 230));
	background.setOutlineThickness(3.f);
	background.setOutlineColor(sf::Color::Yellow);

	title.setString("Leaderboard");
	title.setCharacterSize(40);
	title.setFillColor(sf::Color::White);
	title.setPosition(sf::Vector2f(250.0f, 50.0f ));

	loadingText.setString("Loading Scores...");
	loadingText.setCharacterSize(30);
	loadingText.setFillColor(sf::Color::White);
	loadingText.setPosition(sf::Vector2f(320.0f, 255.0f ));

	for (int i = 0; i < 5; ++i) {
		scoreTexts[i]->setCharacterSize(25);
		scoreTexts[i]->setFillColor(sf::Color::White);
		scoreTexts[i]->setPosition(sf::Vector2f(150.0f, 120.0f + i * 50.0f));
	}

	continueButton.setSize(sf::Vector2f(200.0f, 50.0f ));
	continueButton.setFillColor(sf::Color(50, 150, 50));
	continueButton.setPosition(sf::Vector2f(300.0f, 380.0f));
	continueButton.setOutlineThickness(2.f);
	continueButton.setOutlineColor(sf::Color::White);

	continueText.setString("Continue");
	continueText.setCharacterSize(24);
	continueText.setFillColor(sf::Color::White);
	continueText.setPosition(sf::Vector2f(340.0f, 390.0f));

	waveText.setCharacterSize(30);
	waveText.setFillColor(sf::Color::Yellow);
	waveText.setPosition(sf::Vector2f(280.0f, 140.0f));

	promptName.setString("Enter Name:");
	promptName.setCharacterSize(24);
	promptName.setFillColor(sf::Color::White);
	promptName.setPosition(sf::Vector2f(270.0f, 200.0f));

	inputBox.setSize(sf::Vector2f(300.f, 40.f));
	inputBox.setFillColor(sf::Color(50, 50, 50));
	inputBox.setOutlineThickness(2.f);
	inputBox.setOutlineColor(sf::Color::White);
	inputBox.setFillColor(sf::Color(50, 50, 50));
	inputBox.setPosition(sf::Vector2f(250.0f, 245.0f));

	displayName.setCharacterSize(25);
	displayName.setFillColor(sf::Color::White);	
	displayName.setPosition(sf::Vector2f(260.0f, 250.0f));

	instructionText.setString("Press Enter when done (max 15 character)");
	instructionText.setCharacterSize(16);
	instructionText.setFillColor(sf::Color(200, 200, 200));
	instructionText.setPosition(sf::Vector2f(210.0f, 310.0f));
}

void Leaderboard::updateDisplay() {
	for (int i = 0; i < 5; ++i) {
		if (!topScores[i].Name.empty() && topScores[i].Waves > 0) {
			std::string displayText = std::to_string(i + 1) + ". " + topScores[i].Name + " - Waves: " + std::to_string(topScores[i].Waves);
			scoreTexts[i]->setString(displayText);
		}
		else {
			std::string displayText = std::to_string(i + 1) + ". ---";
			scoreTexts[i]->setString(displayText);
		}
	}
}

void Leaderboard::updateHover(sf::Vector2f mousePos) {
	if (continueButton.getGlobalBounds().contains(mousePos)) {
		continueButton.setFillColor(sf::Color(70, 200, 70));
	}
	else {
		continueButton.setFillColor(sf::Color(50, 150, 50));
	}
}

void Leaderboard::handleNameInput(sf::Event& event) {
	if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
		if (textEntered->unicode == '\b' && !playerName.empty()) {
			// Backspace
			playerName.pop_back();
			displayName.setString(playerName);
		}
		else if (textEntered->unicode == '\r' || textEntered->unicode == '\n') {
			// Enter key
			if (!playerName.empty()) {
				submitScore();
			}
		}
		else if (textEntered->unicode < 128 && textEntered->unicode >= 32) {
			// Regular character
			if (playerName.length() < 15) {
				playerName += static_cast<char>(textEntered->unicode);
				displayName.setString(playerName);
			}
		}
	}
}

void Leaderboard::submitScore() {
	if (manager.SendScore(playerName, currentWaves)) {
		std::cout << "Score successfully sent to Firebase!" << std::endl;
		fetchScores();
		state = LeaderboardState::Displayed;
	}
	else {
		std::cerr << "Failed to send score to Firebase." << std::endl;
		fetchScores();
		state = LeaderboardState::Displayed;
	}
}
