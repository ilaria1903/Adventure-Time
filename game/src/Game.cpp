#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>

#include "../include/Game.h"
#include "../include/Exception.h"

#define TILE_SIZE 16
#define TILE_SCALE 3.0f

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 700

Game::Game()
    : character("Player", 100, 0, 0, 3.0f), level("assets/world/back.png"), editor(level, 16, 16, 3.0f), camera(WINDOW_WIDTH, WINDOW_HEIGHT), deltaTime(0.0f), isFocused(true), ui(), dialogueBox() {
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
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Adventure Time", sf::Style::Default);
        window.setVerticalSyncEnabled(true);

        level.loadTileset("assets/world/tileset.png", TILE_SIZE, TILE_SIZE);

        int width = window.getSize().x / (TILE_SIZE * level.getTileScale()) + 1;
        int height = window.getSize().y / (TILE_SIZE * level.getTileScale()) + 1;
        std::vector<std::vector<int>> data(height, std::vector<int>(width, -1));
        level.setLevelData(data);

        if (data.empty() || data[0].empty()) {
            throw std::runtime_error("Level data is improperly initialized.");
        }

        ui.setWindowSize(window.getSize());
        editor.updateLevelData();
        character.clone();

        propsManager.loadProp("sign", "assets/props/sign.png");
        propsManager.loadProp("cherry", "assets/props/cherry.png");

        propsManager.setWindow(window);
        propsManager.setTileSize(TILE_SIZE, TILE_SIZE, TILE_SCALE);

        level.loadLevelData("assets/levels/level1.txt");
        editor.updateLevelData();

        character.update(0, level.getLevelData(), level.getCollisionData(), level.getTileWidth(), level.getTileHeight());
        camera.startFollowing(character.getPosition());

        Enemy enemy("Enemy", 100, 100);
        enemies.push_back(enemy);

        render();

        while (window.isOpen()) {
            bool shouldExit = false;
            sf::Event e{};
            while (window.pollEvent(e)) {
                switch (e.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    level.updateBackgroundScale(window);
                    break;
                case sf::Event::LostFocus:
                    isFocused = false;
                    break;
                case sf::Event::GainedFocus:
                    isFocused = true;
                    break;
                case sf::Event::KeyPressed:
                    if (e.key.code == sf::Keyboard::Escape)
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
            if (shouldExit) {
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

        if (worldPos.x < 0 || worldPos.y < 0 ||
            worldPos.y / TILE_SIZE >= level.getLevelData().size() ||
            worldPos.x / TILE_SIZE >= level.getLevelData()[0].size()) {
            std::cerr << "Invalid enemy spawn position." << std::endl;
            return;
        }

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

    const int expansionThreshold = 1;
    const int expansionAmount = 10;

    sf::Vector2f playerPos = character.getPosition();
    int playerTileX = playerPos.x / level.getTileWidth();
    int levelWidth = level.getLevelData()[0].size();

    if (playerTileX > levelWidth - expansionThreshold) {
        level.expandLevel(expansionAmount);
        editor.updateLevelData();
    }

    camera.update(deltaTime, playerPos);

    for (auto& enemy : enemies) {
        enemy.update(deltaTime, levelData, collisionData, tileWidth, tileHeight);
    }

    for (const auto& prop : propsManager.getPropInstances()) {
        if (character.getHitbox().intersects(prop.sprite.getGlobalBounds())) {
            if (prop.name == "cherry") {
                propsManager.removePropInstance(prop.sprite.getPosition().x, prop.sprite.getPosition().y);
                ui.updateCherries(ui.getCherryCount() + 1);
            } else if (prop.name == "sign") {
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
            std::vector<Enemy> enemiesToRemove;
            for (auto& enemy : enemies) {
                auto swordHitbox = character.getHitbox();
                swordHitbox.left += character.getFacingRight() ? swordHitbox.width : -swordHitbox.width;
                if (swordHitbox.intersects(enemy.getSprite().getGlobalBounds())) {
                    enemy.takeDamage(10);
                    if (enemy.getHealth() <= 0) {
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
    editor.handleInput(window, character.getPosition());

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f adjustedPos = sf::Vector2f(mousePos.x + character.getPosition().x - window.getSize().x / 2, mousePos.y);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
        propsManager.addPropInstance("cherry", 2.5f, adjustedPos.x, adjustedPos.y);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
        propsManager.addPropInstance("sign", 2.5f, adjustedPos.x, adjustedPos.y);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
        propsManager.removePropInstance(adjustedPos.x, adjustedPos.y);
    }
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "Game(" << game.character << ", " << game.level << ", " << game.camera << ")";
    return os;
}
