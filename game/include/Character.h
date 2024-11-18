#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

#include "../include/CollisionType.h"

class Character {
public:
    enum class State {
        Idle,
        Running,
        Jumping
    };

    Character(const std::string& name, int health, int x, int y, float scale = 1.0f);
    Character(const Character& other);
    Character& operator=(const Character& other);
    ~Character();

    void jump();
    void move(float dx, float dy);
    void takeDamage(int amount);
    void heal(int amount);
    void update(float deltaTime, const std::vector<std::vector<int>>& levelData, const std::vector<CollisionType>& collisionData, int tileWidth, int tileHeight);
    void render(sf::RenderWindow& window);

    int getX() const { return x; }
    int getY() const { return y; }

    const sf::FloatRect getHitbox() const;

    friend std::ostream& operator<<(std::ostream& os, const Character& character);

private:
    std::string name;
    int health;
    int x, y;
    State state;
    float scale;
    bool facingRight;
    bool isAirborne;

    sf::Texture idleTexture;
    sf::Texture jumpTexture;
    sf::Texture runTexture;

    std::vector<sf::Sprite> idleSprites;
    std::vector<sf::Sprite> jumpSprites;
    std::vector<sf::Sprite> runSprites;

    int currentFrame;
    float frameTime;
    float elapsedTime;

    float velocityX;
    float velocityY;
    float gravity;
    int jumpCount;
    const int maxJumps = 2;
    float maxFallSpeed = 800.0f;

    std::chrono::time_point<std::chrono::steady_clock> lastJumpTime;

    void loadAnimation(const std::string& filePath, int frameCount, std::vector<sf::Sprite>& sprites, sf::Texture& texture);
    void updateAnimation(float deltaTime);
    void applyGravity(float deltaTime);
    void handleCollisions(const std::vector<std::vector<int>>& levelData, const std::vector<CollisionType>& collisionData, int tileWidth, int tileHeight);
    void drawHitbox(sf::RenderWindow& window);
};

#endif // CHARACTER_H