#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>

#include "../include/Game.h"
// #include "../include/PropsManager.h"
#include "../include/Exception.h"

#define TILE_SIZE 16
#define TILE_SCALE 3.0f

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 700

Game::Game()
    : character("Player", 100, 0, 0, 3.0f), level("assets/world/back.png"), editor(level, 16, 16, 3.0f), camera(WINDOW_WIDTH, WINDOW_HEIGHT), deltaTime(0.0f), isFocused(true), ui(), dialogueBox() {
    // Load story from file
    std::ifstream file("story.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    story = buffer.str();
}

void Game::start() {
    try {
        ///////////////////////////////////////////////////////////////////////////
        /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Adventure Time", sf::Style::Default);
        ///////////////////////////////////////////////////////////////////////////
        //
        ///////////////////////////////////////////////////////////////////////////
        /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
        /// This is needed so we do not burn the GPU                            ///
        window.setVerticalSyncEnabled(true);                                    ///
        /// window.setFramerateLimit(60);                                       ///
        ///////////////////////////////////////////////////////////////////////////

        // Init stuff
        
        level.loadTileset("assets/world/tileset.png", TILE_SIZE, TILE_SIZE);

        // Fill the screen with empty tiles
        int width = window.getSize().x / (TILE_SIZE * level.getTileScale()) + 1;
        int height = window.getSize().y / (TILE_SIZE * level.getTileScale()) + 1;
        std::vector<std::vector<int>> data(height, std::vector<int>(width, -1));
        level.setLevelData(data);

        ui.setWindowSize(window.getSize());
        
        editor.updateLevelData();

        character.clone(); // For function unused warning

        // Load props
        propsManager.loadProp("sign", "assets/props/sign.png");
        propsManager.loadProp("cherry", "assets/props/cherry.png");

        // Set the window and tile size for the PropsManager
        propsManager.setWindow(window);
        propsManager.setTileSize(TILE_SIZE, TILE_SIZE, TILE_SCALE);

        // Load level data
        level.loadLevelData("assets/levels/level1.txt");
        editor.updateLevelData();

        character.update(0, level.getLevelData(), level.getCollisionData(), level.getTileWidth(), level.getTileHeight());
        camera.startFollowing(character.getPosition());

        // Load enemy and clone it
        Enemy enemy("Enemy", 100, 100);
        enemies.push_back(enemy);

        // Cast cu sens
        // enemies.push_back(*dynamic_cast<Enemy*>(enemy.clone()));
        // enemies.push_back(*dynamic_cast<Enemy*>(enemy.clone()));

        // enemies.push_back(enemy);
        // enemies.push_back(enemy);

        std::cout << "Number of Entity instances: " << Entity::getInstanceCount() << std::endl;

        // Run this for initialisation
        render();

        // Start the game loop
        while(window.isOpen()) {
            bool shouldExit = false;
            sf::Event e{};
            while(window.pollEvent(e)) {
                switch(e.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    std::cout << "New width: " << window.getSize().x << '\n'
                            << "New height: " << window.getSize().y << '\n';
                    level.updateBackgroundScale(window);
                    break;
                case sf::Event::LostFocus:
                    isFocused = false;
                    break;
                case sf::Event::GainedFocus:
                    isFocused = true;
                    break;
                case sf::Event::KeyPressed:
                    if(e.key.code == sf::Keyboard::Escape)
                        shouldExit = true;
                    break;
                case sf::Event::MouseWheelScrolled:
                    editor.handleMouseScroll(e.mouseWheelScroll);
                    break;
                case sf::Event::MouseButtonPressed:
                    handleMouseClick(e.mouseButton);
                    break;
                default:
                    break;
                }
            }
            if(shouldExit) {
                window.close();
                break;
            }

            if (isFocused) {
                deltaTime = clock.restart().asSeconds();

                update();
                handleInput();
                render();
            } else {
                clock.restart();
            }
        }
    } catch (const GameException& e) {
        std::cerr << "Game Exception: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Unknown exception\n";
    }
}

void Game::handleMouseClick(const sf::Event::MouseButtonEvent& mouse) {
    if (mouse.button == sf::Mouse::Middle) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        worldPos.x += character.getPosition().x - window.getSize().x / 2;
        
        // Weird bug where enemy has white box if not pointer
        // Spawn enemy at mouse position
        Enemy* enemy = new Enemy("Enemy", worldPos.x, worldPos.y);
        enemies.push_back(*enemy);
    }
}

void Game::update() {
    auto levelData = level.getLevelData();
    auto collisionData = level.getCollisionData();
    auto tileWidth = level.getTileWidth();
    auto tileHeight = level.getTileHeight();

    character.update(deltaTime, levelData, collisionData, tileWidth, tileHeight);
    
    // Check if the player is approaching the edge of the currently loaded level data
    const int expansionThreshold = 1; // Number of tiles from the edge to trigger expansion
    const int expansionAmount = 10; // Number of columns to expand

    sf::Vector2f playerPos = character.getPosition();
    int playerTileX = playerPos.x / level.getTileWidth();
    int levelWidth = level.getLevelData()[0].size();

    if (playerTileX > levelWidth - expansionThreshold) {
        level.expandLevel(expansionAmount);
        editor.updateLevelData();
    }

    // Update camera position
    camera.update(deltaTime, playerPos);

    for (auto& enemy : enemies) {
        // std::cout << "updating position\n";
        enemy.update(deltaTime, levelData, collisionData, tileWidth, tileHeight);
    }

    // Check for collisions with props
    for (const auto& prop : propsManager.getPropInstances()) {
        if (character.getHitbox().intersects(prop.sprite.getGlobalBounds())) {
            if (prop.name == "cherry") {
                // Pick up cherry
                propsManager.removePropInstance(prop.sprite.getPosition().x, prop.sprite.getPosition().y);
                ui.updateCherries(ui.getCherryCount() + 1);
            } else if (prop.name == "sign") {
                // Show dialogue box
                if (!dialogueBox.isVisible()) {
                    dialogueBox.show("This is a sign!");
                }
            }
        } else 
            dialogueBox.hide();
    }

    dialogueBox.update(deltaTime, camera.getView());
}

void Game::render() {
    window.clear();
    // level.render(window);
    camera.apply(window);
    editor.render(window, character.getPosition());
    propsManager.renderProps(window);
    for (auto& enemy : enemies) {
        enemy.render(window);
    }
    character.render(window);
    dialogueBox.render(window);
    window.setView(window.getDefaultView());
    ui.render(window);
    window.display();
}

void Game::handleInput() {
    float dx = 0.0f, dy = 0.0f;
    float speed = 500.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        character.jump();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        dx -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        dx += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        if (character.attack()) {
            // Check for collision with enemies
            std::vector<Enemy> enemiesToRemove;
            for (auto& enemy : enemies) {
                auto swordHitbox = character.getHitbox();
                swordHitbox.left += character.getFacingRight() ? swordHitbox.width : -swordHitbox.width;
                if (swordHitbox.intersects(enemy.getSprite().getGlobalBounds())) {
                    enemy.takeDamage(10);
                    std::cout << "Enemy health: " << enemy.getHealth() << '\n';
                    if (enemy.getHealth() <= 0) {
                        // enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
                        enemiesToRemove.push_back(enemy);
                    }
                }
            }
            for (size_t i = 0; i < enemiesToRemove.size(); i++) {
                enemies.erase(std::remove(enemies.begin(), enemies.end(), enemiesToRemove[i]), enemies.end());
            }
        }
    }

    character.move(dx, dy);

    // Get player position
    sf::Vector2f playerPos = character.getPosition();

    editor.handleInput(window, playerPos);
    
    // Convert mouse position to world coordinates
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    // Add X player position to mouse position
    sf::Vector2f adjustedPos = sf::Vector2f(mousePos.x + playerPos.x - window.getSize().x / 2, mousePos.y);

    // DEBUG
    // Check current tile index at mouse position
    // std::cout << "Tile index: " << editor.getTileIndex(adjustedPos.x, adjustedPos.y) << '\n';

    // if C is pressed add a cherry prop
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
        propsManager.addPropInstance("cherry", 2.5f, adjustedPos.x, adjustedPos.y);
    }

    // if V is pressed add a sign prop
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
        propsManager.addPropInstance("sign", 2.5f, adjustedPos.x, adjustedPos.y);
    }

    // if B is pressed remove a prop at mouse position
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
        propsManager.removePropInstance(adjustedPos.x, adjustedPos.y);
    }
}

// void Game::readStory() {
//     for (char c : story) {
//         std::cout << c << std::flush;
//         std::this_thread::sleep_for(std::chrono::milliseconds(30));
//     }
// }

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "Game(" << game.character << ", " << game.level << ", " << game.camera << ")";
    return os;
}