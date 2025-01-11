#include "../include/Entity.h"

int Entity::instanceCount = 0;

Entity::Entity() : name("Entity"), x(0), y(0), sprite(), texture(), deltaTime(0.0f) { ++instanceCount; }

Entity::Entity(const std::string& name, int x, int y) : name(name), x(x), y(y), sprite(), texture(), deltaTime(0.0f) { ++instanceCount; }

Entity::~Entity() { --instanceCount; }

int Entity::getInstanceCount() { return instanceCount; }

Enemy::Enemy(const std::string& name, int x, int y) : Entity(name, x, y), levelData(), collisionData(), health(20.0f), speed(140.0f), velocityY(0.0f), direction(1), tileWidth(0), tileHeight(0) {
    if (!texture.loadFromFile("assets/enemy/slimer-idle.png")) {
        throw std::runtime_error("Failed to load enemy texture");
    }
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

void Enemy::update(float _deltaTime) {
    deltaTime = _deltaTime;
}

void Enemy::update(float _deltaTime, const std::vector<std::vector<int>>& _levelData, const std::vector<CollisionType>& _collisionData, int _tileWidth, int _tileHeight) {
    deltaTime = _deltaTime;
    levelData = _levelData;
    collisionData = _collisionData;
    tileWidth = _tileWidth;
    tileHeight = _tileHeight;

    int tileX = (int)(x / tileWidth);

    auto hitbox = sprite.getGlobalBounds();
    int tileYBottom = (int)((hitbox.top + hitbox.height + 30.0f) / tileHeight);

    float tempX = x + speed * direction * deltaTime;
    tileX = (int)(tempX / tileWidth);

    if (tileYBottom >= 0 && tileYBottom < (int)levelData.size()) {
        if (direction == 1 && tileX + 1 < (int)levelData[tileYBottom].size() && levelData[tileYBottom][tileX + 1] == -1) {
            direction *= -1;
        } else if (direction == -1 && tileX >= 0 && levelData[tileYBottom][tileX] == -1) {
            direction *= -1;
        }
    }

    x += speed * direction * deltaTime;

    tileYBottom = (int)((hitbox.top + hitbox.height) / tileHeight);
    if (tileYBottom >= 0 && tileYBottom < (int)levelData.size() && tileX >= 0 && tileX < (int)levelData[tileYBottom].size() && levelData[tileYBottom][tileX] == -1) {
        velocityY += 20.0f;
    } else {
        velocityY = 0.0f;
    }

    y += velocityY * deltaTime;
    sprite.setPosition(x, y);
}

void Enemy::render(sf::RenderWindow& window) {
    sprite.setScale(2.0f, 2.0f);
    window.draw(sprite);
}

Entity* Enemy::clone() const {
    return new Enemy(*this);
}

Boss::Boss(const std::string& name, int x, int y) : Entity(name, x, y), levelData(), collisionData(), tileWidth(0), tileHeight(0) {
    if (!texture.loadFromFile(name)) {
        throw std::runtime_error("Failed to load boss texture");
    }
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

void Boss::update(float _deltaTime) {
    deltaTime = _deltaTime;
}

void Boss::update(float _deltaTime, const std::vector<std::vector<int>>& _levelData, const std::vector<CollisionType>& _collisionData, int _tileWidth, int _tileHeight) {
    deltaTime = _deltaTime;
    levelData = _levelData;
    collisionData = _collisionData;
    tileWidth = _tileWidth;
    tileHeight = _tileHeight;

    x += 50.0f * deltaTime;
    sprite.setPosition(x, y);
}

void Boss::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

Entity* Boss::clone() const {
    return new Boss(*this);
}

FlyingEnemy::FlyingEnemy(const std::string& name, int x, int y) : Entity(name, x, y), levelData(), collisionData(), speedX(100.0f), speedY(50.0f), tileWidth(0), tileHeight(0) {
    if (!texture.loadFromFile("assets/enemy/flying-enemy.png")) {
        throw std::runtime_error("Failed to load flying enemy texture");
    }
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

void FlyingEnemy::update(float _deltaTime) {
    deltaTime = _deltaTime;
}

void FlyingEnemy::update(float _deltaTime, const std::vector<std::vector<int>>& _levelData, const std::vector<CollisionType>& _collisionData, int _tileWidth, int _tileHeight) {
    deltaTime = _deltaTime;
    levelData = _levelData;
    collisionData = _collisionData;
    tileWidth = _tileWidth;
    tileHeight = _tileHeight;

    if (tileWidth > 0 && tileHeight > 0) {
        x += speedX * deltaTime;
        y += speedY * deltaTime;
    }

    std::cout << "FlyingEnemy position: " << x << ", " << y << '\n';
    sprite.setPosition(x, y);
}

void FlyingEnemy::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

Entity* FlyingEnemy::clone() const {
    return new FlyingEnemy(*this);
}
