#include "../include/Entity.h"

int Entity::instanceCount = 0;

Entity::Entity() : name("Entity"), health(0.0f), x(0), y(0), sprite(), texture(), deltaTime(0.0f) { ++instanceCount; }

Entity::Entity(const std::string& name, float health, int x, int y) : name(name), health(health), x(x), y(y), sprite(), texture(), deltaTime(0.0f) { ++instanceCount; }

Entity::~Entity() { --instanceCount; }

int Entity::getInstanceCount() { return instanceCount; }

Enemy::Enemy(const std::string& name, int x, int y) : Entity(name, 20.0f, x, y), levelData(), collisionData(), speed(140.0f), velocityY(0.0f), direction(1), tileWidth(0), tileHeight(0), currentFrame(0), frameTime(0.2f), elapsedTime(0.0f), frameWidth(41), frameHeight(38), numFrames(8) {
    loadSpritesheet();
    sprite.setTexture(spritesheet);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setPosition(x, y);
}

void Enemy::loadSpritesheet() {
    if (!spritesheet.loadFromFile("assets/enemy/slimer-idle.png")) {
        throw std::runtime_error("Failed to load enemy spritesheet");
    }
}

void Enemy::takeDamage(float amount) {
    health -= amount;
}

void Enemy::move(float dx, float dy) {
    x += dx;
    y += dy;
    sprite.setPosition(x, y);
}

void Enemy::update(float _deltaTime) {
    deltaTime = _deltaTime;
    elapsedTime += deltaTime;

    if (elapsedTime >= frameTime) {
        elapsedTime = 0.0f;
        currentFrame = (currentFrame + 1) % numFrames;
        int left = currentFrame * frameWidth;
        sprite.setTextureRect(sf::IntRect(left, 0, frameWidth, frameHeight));
    }
}

void Enemy::update(float _deltaTime, const std::vector<std::vector<int>>& _levelData, const std::vector<CollisionType>& _collisionData, int _tileWidth, int _tileHeight) {
    update(_deltaTime);

    // deltaTime = _deltaTime;
    levelData = _levelData;
    collisionData = _collisionData;
    tileWidth = _tileWidth;
    tileHeight = _tileHeight;

    // Update enemy logic
    
    int tileX = (int)(x / tileWidth);

    auto hitbox = sprite.getGlobalBounds();
    int tileYBottom = (int)((hitbox.top + hitbox.height + 30.0f) / tileHeight);
    
    float tempX = x + speed * direction * deltaTime;
    tileX = (int)(tempX / tileWidth);

    // Check if enemy is about to fall off a platform
    if (tileYBottom < (int)levelData.size()) {
        if (direction == 1 && levelData[tileYBottom][tileX + 1] == -1) {
            direction *= -1;
        }
        else if (direction == -1 && levelData[tileYBottom][tileX] == -1) {
            direction *= -1;
        }
    }

    // x += speed * direction * deltaTime;
    
    tileYBottom = (int)((hitbox.top + hitbox.height) / tileHeight);
    // Apply gravity if enemy is not on the ground
    if (tileYBottom < (int)levelData.size() && levelData[tileYBottom][tileX] == -1) {
        velocityY += 20.0f;
    }
    else {
        velocityY = 0.0f;
    }

    // y += velocityY * deltaTime;

    // std::cout << "Enemy position: " << x << ", " << y << '\n';
    // sprite.setPosition(x, y);

    // Move enemy from base class using virtual function
    Entity* enemy = this;
    enemy->move(speed * direction * deltaTime, velocityY * deltaTime);
}

void Enemy::render(sf::RenderWindow& window) {
    // Scale the sprite 2x
    sprite.setScale(2.0f, 2.0f);
    window.draw(sprite);
}

Entity* Enemy::clone() const {
    return new Enemy(*this);
}

Boss::Boss(const std::string& name, int x, int y) : Entity(name, 80.0f, x, y), levelData(), collisionData(), tileWidth(0), tileHeight(0), currentFrame(0), frameTime(0.3f), elapsedTime(0.0f), frameWidth(144), frameHeight(64), numFrames(6) {
    loadSpritesheet();
    sprite.setTexture(spritesheet);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setPosition(x, y);
}

void Boss::loadSpritesheet() {
    if (!spritesheet.loadFromFile("assets/boss/idle.png")) {
        throw std::runtime_error("Failed to load boss spritesheet");
    }
}

void Boss::takeDamage(float amount) {
    health -= amount;
}

void Boss::move(float dx, float dy) {
    x += dx;
    y += dy;
    sprite.setPosition(x, y);
}


void Boss::update(float _deltaTime) {
    deltaTime = _deltaTime;
    elapsedTime += deltaTime;

    if (elapsedTime >= frameTime) {
        elapsedTime = 0.0f;
        currentFrame = (currentFrame + 1) % numFrames;
        int left = currentFrame * frameWidth;
        sprite.setTextureRect(sf::IntRect(left, 0, frameWidth, frameHeight));
    }
}

void Boss::update(float _deltaTime, const std::vector<std::vector<int>>& _levelData, const std::vector<CollisionType>& _collisionData, int _tileWidth, int _tileHeight) {
    update(_deltaTime);

    levelData = _levelData;
    collisionData = _collisionData;
    tileWidth = _tileWidth;
    tileHeight = _tileHeight;

    // Update boss logic
    // x += 50.0f * deltaTime;
    // sprite.setPosition(x, y);
}

void Boss::render(sf::RenderWindow& window) {
    // Scale the sprite 3x
    sprite.setScale(3.0f, 3.0f);
    window.draw(sprite);
}

Entity* Boss::clone() const {
    return new Boss(*this);
}

FlyingEnemy::FlyingEnemy(const std::string& name, int x, int y) : Entity(name, 20.0f, x, y), levelData(), collisionData(), direction(0), multiplier(1), speedX(200.0f), speedY(50.0f), tileWidth(0), tileHeight(0),  currentFrame(0), frameTime(0.2f), elapsedTime(0.0f), frameWidth(53), frameHeight(57), numFrames(3) {
    loadSpritesheet();
    sprite.setTexture(spritesheet);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setPosition(x, y);
}

void FlyingEnemy::loadSpritesheet() {
    if (!spritesheet.loadFromFile("assets/flying-enemy/bat-sheet.png")) {
        throw std::runtime_error("Failed to load flying enemy spritesheet");
    }
}

void FlyingEnemy::takeDamage(float amount) {
    health -= amount;
}

void FlyingEnemy::move(float dx, float dy) {
    x += dx;
    y += dy;
    sprite.setPosition(x, y);
}

void FlyingEnemy::update(float _deltaTime) {
    deltaTime = _deltaTime;
    elapsedTime += deltaTime;

    if (elapsedTime >= frameTime) {
        elapsedTime = 0.0f;
        currentFrame = (currentFrame + 1) % numFrames;
        int left = currentFrame * frameWidth;
        sprite.setTextureRect(sf::IntRect(left, 0, frameWidth, frameHeight));
    }
}

void FlyingEnemy::update(float _deltaTime, const std::vector<std::vector<int>>& _levelData, const std::vector<CollisionType>& _collisionData, int _tileWidth, int _tileHeight) {
    update(_deltaTime);

    // deltaTime = _deltaTime;
    levelData = _levelData;
    collisionData = _collisionData;
    tileWidth = _tileWidth;
    tileHeight = _tileHeight;

    if (direction > 100) {
        multiplier *= -1;
        direction = 0;
    }
    else {
        direction += 1;
    }

    move(speedX * deltaTime * multiplier, speedY * deltaTime);
}

void FlyingEnemy::render(sf::RenderWindow& window) {
    // Scale the sprite 2x
    sprite.setScale(2.0f, 2.0f);
    window.draw(sprite);
}

Entity* FlyingEnemy::clone() const {
    return new FlyingEnemy(*this);
}