#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Level.h"
#include "Camera.h"
#include "LevelEditor.h"
#include "Interface.h"
#include "DialogueBox.h"
#include "PropsManager.h"

#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>

using namespace std::chrono_literals;

class Game {
public:
    Game();

    void start();
    void update();
    void render();
    void handleInput();
    void handleMouseClick(const sf::Event::MouseButtonEvent& mouse);
    // void readStory();

    friend std::ostream& operator<<(std::ostream& os, const Game& game);

private:
    Player character;
    Level level;
    LevelEditor editor;
    Camera camera;
    sf::RenderWindow window;
    std::string story;
    sf::Clock clock;
    float deltaTime;
    bool isFocused;
    Interface ui;
    DialogueBox dialogueBox;
    std::vector<Enemy> enemies;
    PropsManager propsManager;
};

#endif // GAME_H