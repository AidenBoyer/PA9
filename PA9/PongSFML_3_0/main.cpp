#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include "MainMenu.hpp"
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include "AudioManager.hpp"
#include "WaveSystem.hpp"
#include "CollisionEngine.hpp"
#include "LeaderboardManager.hpp"
#include "LeaderBoard.hpp"

#include "LeaderBoard.hpp"


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
    int totalWavesCompleted = 0;

    leaderboard.fetchScores();
    AudioManager AudManager;

    std::list<Bullet> playerBullets;
    std::list<Bullet> enemyBullets;


    sf::Texture playerBulletTexture;
    playerBulletTexture.loadFromFile("Bullet.png");
    Bullet playerMasterBullet(playerBulletTexture);
    playerMasterBullet.setScale(sf::Vector2f(.5, .5));

    sf::Texture enemyBulletTexture;
    enemyBulletTexture.loadFromFile("Bullet.png");
    Bullet enemyMasterBullet(enemyBulletTexture);
    enemyMasterBullet.setScale(sf::Vector2f(.5, .5));

    sf::Texture playerTexture;
    playerTexture.loadFromFile("REAL SHIP.png");

    Player player(playerTexture, playerMasterBullet);
    player.initialize(window.getSize());

    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("Enemy.png");
    //Enemy enemy1(enemyTexture);
    WaveSystem waveSystem(enemyTexture, enemyMasterBullet);
    waveSystem.spawnWave(1, window.getSize());



    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("Background.png");
    sf::Sprite backgroundSprite1(backgroundTexture);
    sf::Sprite backgroundSprite2(backgroundTexture);
    auto winSize = window.getSize();
    const float scaleX = static_cast<float>(winSize.x) / backgroundTexture.getSize().x;
    const float scaleY = static_cast<float>(winSize.y) / backgroundTexture.getSize().y;
    backgroundSprite1.setScale(sf::Vector2f(scaleX, scaleY));
    backgroundSprite2.setScale(sf::Vector2f(scaleX, scaleY));
    backgroundSprite1.setPosition(sf::Vector2f(0.f, 0.f));
    backgroundSprite2.setPosition(sf::Vector2f(0.f, -static_cast<float>(winSize.y)));
    float backgroundScrollSpeed = 100.f;


    sf::Clock clock;
    window.setFramerateLimit(200);
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

                window.setSize({ width, newHeight });

                window.setView(sf::View(sf::FloatRect({ 0.0f, 0.0f }, { static_cast<float>(width),  static_cast<float>(newHeight) })));
                waveSystem.updateLayout(window.getSize());
                player.updateLayout(window.getSize());


                const float newScaleX = static_cast<float>(width) / backgroundTexture.getSize().x;
                const float newScaleY = static_cast<float>(newHeight) / backgroundTexture.getSize().y;
                backgroundSprite1.setScale(sf::Vector2f(newScaleX, newScaleY));
                backgroundSprite2.setScale(sf::Vector2f(newScaleX, newScaleY));
                backgroundSprite1.setPosition(sf::Vector2f(0.f, 0.f));
                backgroundSprite2.setPosition(sf::Vector2f(0.f, -static_cast<float>(newHeight)));

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
                    totalWavesCompleted = 0; // Reset wave counter when starting new game
                }
            }
            else if (currentState == GameState::Leaderboard) {
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
            AudManager.playMenuMusic();
        }
        else if (currentState == GameState::Playing) {
            // update all
            AudManager.playIngameMusic();
            
            // Check if wave was completed before updating
            auto& enemies = waveSystem.getEnemies();
            bool waveWasEmpty = enemies.empty();
            
            waveSystem.updateEnemies(dt);
            
            // If wave was empty and now has enemies, a new wave was spawned
            if (waveWasEmpty && !waveSystem.getEnemies().empty()) {
                totalWavesCompleted++;
            }
            
            // Player firing
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && player.getFireCooldown() == 0.0) {
                playerBullets.push_back(player.fire());
                AudManager.playShoot();
            }
            
            // Enemy firing
            for (auto& enemy : enemies) {
                if (enemy.isAlive()) {
                    enemy.update(dt);
                    if (enemy.getFireCooldown() == 0.0) {
                        enemyBullets.push_back(enemy.fire());
                    }
                }
            }
            
            // Update player bullets
            for (auto it = playerBullets.begin(); it != playerBullets.end();) {
                if (!it->isAlive()) {
                    it = playerBullets.erase(it);
                }
                else {
                    it->update(dt);
                    // Remove bullets that go off screen
                    if (it->getPosition().y < -100.f || it->getPosition().y > window.getSize().y + 100.f) {
                        it = playerBullets.erase(it);
                    }
                    else {
                        ++it;
                    }
                }
            }
            
            // Update enemy bullets
            for (auto it = enemyBullets.begin(); it != enemyBullets.end();) {
                if (!it->isAlive()) {
                    it = enemyBullets.erase(it);
                }
                else {
                    it->update(dt);
                    // Remove bullets that go off screen
                    if (it->getPosition().y < -100.f || it->getPosition().y > window.getSize().y + 100.f) {
                        it = enemyBullets.erase(it);
                    }
                    else {
                        ++it;
                    }
                }
            }
            
            if (player.isAlive()) {
                player.update(dt, window);
            }
            else {
                // Player died - transition to leaderboard
                currentState = GameState::Leaderboard;
                leaderboard.inputName(totalWavesCompleted);
            }
            // collide game objects
            // TODO




            CollisionEngine::applyCollisions(AudManager, player, playerBullets, enemies, enemyBullets);
            for (std::size_t i = enemies.size(); i > 0; --i) {
                if (!enemies[i - 1].isAlive()) {
                    enemies.erase(enemies.begin() + (i - 1));
                }
            }
        }
        else if (currentState == GameState::Leaderboard) {
            leaderboard.update(window);
        }

        window.clear();

        //Different render depending on game state
        if (currentState == GameState::Menu) {
            mainMenu.drawMenu(window);
        }
        else if (currentState == GameState::Playing) {
            float dy = backgroundScrollSpeed * dt;
            backgroundSprite1.move(sf::Vector2f(0.f, dy));
            backgroundSprite2.move(sf::Vector2f(0.f, dy));

            // Use the current view/window height for wrapping
            const float wrapHeight = window.getView().getSize().y;

            // Wrap sprites when they move off the bottom
            if (backgroundSprite1.getPosition().y >= wrapHeight) {
                backgroundSprite1.setPosition(sf::Vector2f(0.f, backgroundSprite2.getPosition().y - wrapHeight));
            }
            if (backgroundSprite2.getPosition().y >= wrapHeight) {
                backgroundSprite2.setPosition(sf::Vector2f(0.f, backgroundSprite1.getPosition().y - wrapHeight));
            }
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            for (const auto& enemy : waveSystem.getEnemies()) {
                if (enemy.isAlive()) {
                    window.draw(enemy);
                }
            }
            if (player.isAlive()) {
                window.draw(player);
            }
            for (auto& bullet : playerBullets) {
                window.draw(bullet);
            }
            for (auto& bullet : enemyBullets) {
                window.draw(bullet);
            }
        }
        else if (currentState == GameState::Leaderboard) {
            leaderboard.draw(window);
        }

        window.display();
    }
}