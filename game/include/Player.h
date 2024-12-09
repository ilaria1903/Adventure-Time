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
#include "../include/Entity.h"

class Player : public Entity {
public:
    enum class State {
        Idle,
        Running,
        Jumping,
        Attacking
    };

    Player(const std::string& name, int health, int x, int y, float scale = 1.0f);
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player() override;

    void jump();
    void move(float dx, float dy);
    // void takeDamage(int amount);
    // void heal(int amount);
    void attack();
    void update(float deltaTime) override;
    void update(float deltaTime, const std::vector<std::vector<int>>& levelData, const std::vector<CollisionType>& collisionData, int tileWidth, int tileHeight) override;
    Entity* clone() const override;
    void render(sf::RenderWindow& window) override;

    const sf::Vector2f getVelocity() const;
    const sf::Vector2f getPosition() const;
    // int getX() const { return x; }
    // int getY() const { return y; }

    const sf::FloatRect getHitbox() const;
    // State getState() const { return state; }

    // void setHealth(int health) { this->health = health; }
    // int getHealth() const { return health; }

    friend std::ostream& operator<<(std::ostream& os, const Player& character);

private:
    int health;
    State state;
    float scale;
    bool facingRight;
    bool isAirborne;

    sf::Texture idleTexture;
    sf::Texture jumpTexture;
    sf::Texture runTexture;
    sf::Texture attackTexture;

    std::vector<sf::Sprite> idleSprites;
    std::vector<sf::Sprite> jumpSprites;
    std::vector<sf::Sprite> runSprites;
    std::vector<sf::Sprite> attackSprites;

    sf::Sprite slashSprite;

    int currentFrame;
    float frameTime;
    float elapsedTime;
    float lastAttackTime;
    float velocityX;
    float velocityY;
    float gravity;
    int jumpCount;
    int maxJumps;
    float maxFallSpeed;

    sf::RectangleShape rect1, rect2;

    std::chrono::time_point<std::chrono::steady_clock> lastJumpTime;

    void loadAnimation(const std::string& filePath, int frameCount, std::vector<sf::Sprite>& sprites, sf::Texture& texture);
    void updateAnimation(float deltaTime);
    void applyGravity(float deltaTime);
    void handleCollisions(const std::vector<std::vector<int>>& levelData, const std::vector<CollisionType>& collisionData, int tileWidth, int tileHeight);
    // void drawHitbox(sf::RenderWindow& window);
};

#endif // CHARACTER_H