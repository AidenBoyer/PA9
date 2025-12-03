#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include "MainMenu.hpp"
#include <iostream>
#include <filesystem>
#include "AudioManager.hpp"
#include "WaveSystem.hpp"

enum class GameState {
    Menu,
    Playing
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Space Defender");


    
    const float aspectRatio = 16.0f / 9.0f;

    GameState currentState = GameState::Menu;
    MainMenu mainMenu;
    
    AudioManager AudManager;
    AudManager.playShoot();

    sf::Texture playerTexture;
    std::cout << std::filesystem::current_path() << std::endl;
    playerTexture.loadFromFile("Ship.png");
    Player player(playerTexture);

    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("Enemy.png");
    Enemy enemy1(enemyTexture);
	WaveSystem waveSystem(enemyTexture);
	waveSystem.spawnWave(1, window.getSize());

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
				waveSystem.spawnWave(1, window.getSize());
            }

            if (currentState == GameState::Menu) {
                if (mainMenu.click(window, event.value())) {
                    currentState = GameState::Playing; //transition to actual game here
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
			waveSystem.updateEnemies(dt);
        }

        window.clear();
        
        //Different render depending on game state
        if (currentState == GameState::Menu) {
            mainMenu.drawMenu(window);
        }
        else if (currentState == GameState::Playing) {
            for (const auto& enemy : waveSystem.getEnemies()) {
                window.draw(enemy);
            }
            window.draw(player);
            window.draw(enemy1);
        }
        
        window.display();
    }
}