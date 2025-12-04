#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "MainMenu.hpp"
#include "LeaderBoard.hpp"
#include <iostream>
#include <filesystem>

enum class GameState {
    Menu,
    Playing,
    Leaderboard
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Space Defender");
    
    const float aspectRatio = 16.0f / 9.0f;
    const float gameWidth = 800.0f;
    const float gameHeight = gameWidth / aspectRatio;
    const sf::Vector2f gameSize(gameWidth, gameHeight);

    GameState currentState = GameState::Menu;
    MainMenu mainMenu;

    Leaderboard leaderboard("https://cpts122pa9-default-rtdb.firebaseio.com/leaderboard.json");
    int currentWaves = 0;

	leaderboard.fetchScores();

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

            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                unsigned int width = resized->size.x;
                unsigned int height = resized->size.y;

                float windowRatio = static_cast<float>(width) / static_cast<float>(height);
                sf::FloatRect viewport;

                if (windowRatio > aspectRatio) {
                   
                    float viewportWidth = aspectRatio / windowRatio;
                    viewport = sf::FloatRect({ (1.0f - viewportWidth) / 2.0f, 0.0f },
                        { viewportWidth, 1.0f });
                }
                else {
                    float viewportHeight = windowRatio / aspectRatio;
                    viewport = sf::FloatRect({ 0.0f, (1.0f - viewportHeight) / 2.0f },
                        { 1.0f, viewportHeight });
                }

                sf::View view(sf::FloatRect({ 0.0f, 0.0f }, gameSize));
                view.setViewport(viewport);
                window.setView(view);
            }

            if (currentState == GameState::Menu) {
                if (mainMenu.click(window, event.value())) {
                    currentState = GameState::Playing; //transition to actual game here
                }
            }

            if (currentState == GameState::Leaderboard) {
                if (leaderboard.handleEvents(window, event.value())) {
                    currentState = GameState::Menu;
                    leaderboard.reset();
                }
            }
        }

        float dt = clock.restart().asSeconds();
        
        //Different update depending on game state
        if (currentState == GameState::Menu) {
            mainMenu.update(window);
        }
        else if (currentState == GameState::Playing) {
            player.update(dt);
        }
        else {
			leaderboard.update(window);
        }

        window.clear();
        
        //Different render depending on game state
        if (currentState == GameState::Menu) {
            mainMenu.drawMenu(window);
        }
        else if (currentState == GameState::Playing) {
            window.draw(player);
        }
        else {
			leaderboard.draw(window);
        }
        
        window.display();
    }
}