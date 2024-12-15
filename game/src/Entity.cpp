#include "../include/Entity.h"

Entity::Entity() : name("Entity"), x(0), y(0), sprite(), texture(), deltaTime(0.0f) {}

Entity::Entity(const std::string& name, int x, int y) : name(name), x(x), y(y), sprite(), texture(), deltaTime(0.0f) {}

//Enemy::Enemy(const std::string& name, int x, int y) : Entity(name, x, y), levelData(), collisionData(), tileWidth(0), tileHeight(0) {
//    if (!texture.loadFromFile("assets/enemy/slimer-idle.png")) {
//        throw std::runtime_error("Failed to load enemy texture");
//    }
 //   sprite.setTexture(texture);
//    sprite.setPosition(x, y);
//}

//void Enemy::update(float _deltaTime) {
//    deltaTime = _deltaTime;
//}

//void Enemy::update(float _deltaTime, const std::vector<std::vector<int>>& _levelData, const std::vector<CollisionType>& _collisionData, int _tileWidth, int _tileHeight) {
   // deltaTime = _deltaTime;
 //   levelData = _levelData;
 //   collisionData = _collisionData;
 //   tileWidth = _tileWidth;
 //   tileHeight = _tileHeight;

    // Update enemy logic
 //   x += 50.0f * deltaTime;
 //   sprite.setPosition(x, y);
//}

//void Enemy::render(sf::RenderWindow& window) {
 //   window.draw(sprite);
//}

//Entity* Enemy::clone() const {
 //   return new Enemy(*this);
//}

//Boss::Boss(const std::string& name, int x, int y) : Entity(name, x, y), levelData(), collisionData(), tileWidth(0), tileHeight(0) {
//    if (!texture.loadFromFile(name)) {
//        throw std::runtime_error("Failed to load boss texture");
 //   }
 //   sprite.setTexture(texture);
 //   sprite.setPosition(x, y);
//}

//void Boss::update(float _deltaTime) {
 //   deltaTime = _deltaTime;
//}

//void Boss::update(float _deltaTime, const std::vector<std::vector<int>>& _levelData, const std::vector<CollisionType>& _collisionData, int _tileWidth, int _tileHeight) {
 //   deltaTime = _deltaTime;
 //   levelData = _levelData;
 //   collisionData = _collisionData;
 //   tileWidth = _tileWidth;
 //   tileHeight = _tileHeight;

    // Update enemy logic
 //   x += 50.0f * deltaTime;
 //   sprite.setPosition(x, y);
//}

//void Boss::render(sf::RenderWindow& window) {
 //   window.draw(sprite);
//}

//Entity* Boss::clone() const {
//    return new Boss(*this);
//}