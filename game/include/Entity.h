#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <string>

#include "CollisionType.h"
#include <vector>
#include <iostream>

class Entity {
public:
    Entity();
    Entity(const std::string& name, int x, int y);
    virtual ~Entity() = default;

    virtual void update(float deltaTime) = 0;
    virtual void update(float deltaTime, const std::vector<std::vector<int>>& levelData, const std::vector<CollisionType>& collisionData, int tileWidth, int tileHeight) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual Entity* clone() const = 0;

protected:
    std::string name;
    int x, y;
    sf::Sprite sprite;
    sf::Texture texture;
    float deltaTime;
};

class Enemy : public Entity {
public:
    Enemy(const std::string& name, int x, int y);
    void update(float deltaTime) override;
    void update(float deltaTime, const std::vector<std::vector<int>>& levelData, const std::vector<CollisionType>& collisionData, int tileWidth, int tileHeight) override;
    void render(sf::RenderWindow& window) override;
    Entity* clone() const override;
private:
    std::vector<std::vector<int>> levelData;
    std::vector<CollisionType> collisionData;
    int tileWidth;
    int tileHeight;
};

class Boss : public Entity {
public:
    Boss(const std::string& name, int x, int y);
    void update(float deltaTime) override;
    void update(float deltaTime, const std::vector<std::vector<int>>& levelData, const std::vector<CollisionType>& collisionData, int tileWidth, int tileHeight) override;
    void render(sf::RenderWindow& window) override;
    Entity* clone() const override;
private:
    std::vector<std::vector<int>> levelData;
    std::vector<CollisionType> collisionData;
    int tileWidth;
    int tileHeight;
};

#endif // ENTITY_H