#ifndef GAME_H
#define GAME_H

#include "Character.h"
#include "Level.h"
#include "Camera.h"
#include "LevelEditor.h"

#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

using namespace std::chrono_literals;

class Game {
public:
    Game();
    Game(const Game& other);

    void start();
    void update();
    void render();
    void handleInput();
    void readStory();

    friend std::ostream& operator<<(std::ostream& os, const Game& game);

private:
    Character character;
    Level level;
    LevelEditor editor;
    Camera camera;
    sf::RenderWindow window;
    std::string story;
    sf::Clock clock;
    float deltaTime;
    bool isFocused;
};

#endif // GAME_H