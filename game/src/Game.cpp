#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>

#include "../include/Game.h"
// #include "../include/Character.h"
// #include "../include/Level.h"
// #include "../include/LevelEditor.h"

Game::Game()
    : character("Player", 100, 0, 0, 3.0f), level("..//game//assets//world//back.png"), editor(level, 16, 16, 3.0f), camera(0, 0), deltaTime(0.0f), isFocused(true) {
    // Load story from file
    std::ifstream file("..//story.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    story = buffer.str();
}

Game::Game(const Game& other)
    : character(other.character), level(other.level), camera(other.camera), story(other.story), deltaTime(other.deltaTime), editor(other.editor) {}

void Game::start() {
    // readStory();

    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:31
    window.create(sf::VideoMode({800, 700}), "Adventure Time", sf::Style::Default);
    ///////////////////////////////////////////////////////////////////////////
    //
    ///////////////////////////////////////////////////////////////////////////
    /// NOTE: mandatory use one of vsync or FPS limit (not both)            ///
    /// This is needed so we do not burn the GPU                            ///
    window.setVerticalSyncEnabled(true);                                    ///
    /// window.setFramerateLimit(60);                                       ///
    ///////////////////////////////////////////////////////////////////////////

    // Init stuff
    
    level.loadTileset("..//game//assets//world//tileset.png", 16, 16);

    // Fill the screen with empty tiles
    int width = window.getSize().x / (16 * level.getTileScale()) + 1;
    int height = window.getSize().y / (16 * level.getTileScale()) + 1;
    std::vector<std::vector<int>> data(height, std::vector<int>(width, -1));
    level.setLevelData(data);
    
    editor.updateLevelData();

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
                // std::cout << "Received key " << (e.key.code == sf::Keyboard::X ? "X" : "(other)") << "\n";
                if(e.key.code == sf::Keyboard::Escape)
                    shouldExit = true;
                break;
            case sf::Event::MouseWheelScrolled:
                editor.handleMouseScroll(e.mouseWheelScroll);
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

            handleInput();
            update();
            render();
        } else {
            clock.restart();
        }
    }
}

void Game::update() {
    character.update(deltaTime, level.getLevelData(), level.getCollisionData(), level.getTileWidth(), level.getTileHeight());
    // camera.follow(character.getX(), character.getY());
}

void Game::render() {
    window.clear();
    // level.render(window);
    editor.render(window);
    character.render(window);
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

    character.move(dx, dy);
    
    editor.handleInput(window);
}

void Game::readStory() {
    for (char c : story) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "Game(" << game.character << ", " << game.level << ", " << game.camera << ")";
    return os;
}