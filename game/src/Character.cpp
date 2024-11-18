#include "../include/Character.h"

Character::Character(const std::string& name, int health, int x, int y, float scale)
    : name(name), health(health), x(x), y(y), state(State::Idle), scale(scale), facingRight(true), isAirborne(true), currentFrame(0), frameTime(0.1f), elapsedTime(0.0f),
      velocityX(0.0f), velocityY(0.0f), gravity(1200.0f), jumpCount(0) { // Gravity in pixels per second squared
    loadAnimation("../game/assets/player/sunny-bunny-idle.png", 4, idleSprites, idleTexture);
    loadAnimation("../game/assets/player/sunny-bunny-jump.png", 5, jumpSprites, jumpTexture);
    loadAnimation("../game/assets/player/sunny-bunny-run.png", 6, runSprites, runTexture);
}

Character::Character(const Character& other)
    : name(other.name), health(other.health), x(other.x), y(other.y), state(other.state), scale(other.scale), facingRight(other.facingRight), isAirborne(other.isAirborne), currentFrame(other.currentFrame),
      frameTime(other.frameTime), elapsedTime(other.elapsedTime), idleSprites(other.idleSprites),
      jumpSprites(other.jumpSprites), runSprites(other.runSprites), velocityX(other.velocityX), velocityY(other.velocityY), gravity(other.gravity), jumpCount(other.jumpCount) {}

Character& Character::operator=(const Character& other) {
    if (this != &other) {
        name = other.name;
        health = other.health;
        x = other.x;
        y = other.y;
        state = other.state;
        scale = other.scale;
        facingRight = other.facingRight;
        isAirborne = other.isAirborne;
        currentFrame = other.currentFrame;
        frameTime = other.frameTime;
        elapsedTime = other.elapsedTime;
        idleSprites = other.idleSprites;
        jumpSprites = other.jumpSprites;
        runSprites = other.runSprites;
        velocityX = other.velocityX;
        velocityY = other.velocityY;
        gravity = other.gravity;
        jumpCount = other.jumpCount;
    }
    return *this;
}

Character::~Character() {}

const sf::FloatRect Character::getHitbox() const {
    return sf::FloatRect(x, y, idleSprites[0].getGlobalBounds().width, idleSprites[0].getGlobalBounds().height);
}

void Character::jump() {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastJumpTime).count();

    if (elapsedTime < 500) {
        return;
    }

    if (jumpCount < maxJumps) {
        y -= 3.0f; // Misca un pic in sus pt a preveni coliziunea cu pamantul
        velocityY = -800.0f;
        jumpCount++;
        lastJumpTime = currentTime;
        isAirborne = true;
    }
}

void Character::move(float dx, float dy) {
    // x += dx;
    // y += dy;

    // velocityX = dx;

    velocityX = dx;
    velocityY += dy;

    if (dx > 0) {
        facingRight = true;
    } else if (dx < 0) {
        facingRight = false;
    }
}

void Character::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void Character::heal(int amount) {
    health += amount;
}

void Character::update(float deltaTime, const std::vector<std::vector<int>>& levelData, const std::vector<CollisionType>& collisionData, int tileWidth, int tileHeight) {
    // Verifica colizune cu josul ecranului
    auto hitbox = getHitbox();
    State oldState = state;

    // DEBUG
    std::cout << "isAirborne: " << isAirborne << std::endl;

    applyGravity(deltaTime);
    if (isAirborne) {
        state = State::Jumping;
    } else {
        if (velocityX == 0) {
            state = State::Idle;
        } else {
            state = State::Running;
        }
    }

    if (oldState != state) {
        currentFrame = 0;
    }

    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    handleCollisions(levelData, collisionData, tileWidth, tileHeight);

    updateAnimation(deltaTime);
}

void Character::drawHitbox(sf::RenderWindow& window) {
    sf::RectangleShape rect;
    sf::FloatRect hitbox = getHitbox();
    rect.setSize(sf::Vector2f(hitbox.width, hitbox.height));
    rect.setPosition(hitbox.left, hitbox.top);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(1.0f);

    // Ajusteaza pozitia hitboxului pe baza directiei sprite ului
    if (!facingRight) {
        rect.setPosition(hitbox.left - hitbox.width, hitbox.top);
    }

    window.draw(rect);
}

void Character::render(sf::RenderWindow& window) {
    sf::Sprite* currentSprite = nullptr;

    switch (state) {
        case State::Idle:
            if (currentFrame >= idleSprites.size()) currentFrame = 0;
            currentSprite = &idleSprites[currentFrame];
            break;
        case State::Running:
            if (currentFrame >= runSprites.size()) currentFrame = 0;
            currentSprite = &runSprites[currentFrame];
            break;
        case State::Jumping:
            if (currentFrame >= jumpSprites.size()) currentFrame = 0;
            currentSprite = &jumpSprites[currentFrame];
            break;
    }

    // DEBUG
    // drawHitbox(window);

    if (currentSprite) {
        currentSprite->setPosition(x, y);
        if (facingRight) {
            currentSprite->setScale(scale, scale);
        } else {
            currentSprite->setScale(-scale, scale);
        }
        window.draw(*currentSprite);
    }
}

void Character::loadAnimation(const std::string& filePath, int frameCount, std::vector<sf::Sprite>& sprites, sf::Texture& texture) {
    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return;
    }

    int frameWidth = texture.getSize().x / frameCount;
    int frameHeight = texture.getSize().y;

    for (int i = 0; i < frameCount; ++i) {
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(i * frameWidth, 0, frameWidth, frameHeight));
        sprite.setScale(scale, scale);
        sprites.push_back(sprite);
    }
}

void Character::updateAnimation(float deltaTime) {
    elapsedTime += deltaTime;
    if (elapsedTime >= frameTime) {
        elapsedTime = 0.0f;

        switch (state) {
            case State::Idle:
                currentFrame = (currentFrame + 1) % idleSprites.size();
                break;
            case State::Running:
                currentFrame = (currentFrame + 1) % runSprites.size();
                break;
            case State::Jumping:
                currentFrame = (currentFrame + 1) % jumpSprites.size();
                if (velocityY < -100.0f) {
                    currentFrame = 1; // Thrusting upwards
                } else if (velocityY > 100.0f) {
                    currentFrame = 3; // Falling down
                } else {
                    currentFrame = 2; // Midair
                }
                break;
        }
    }
}

void Character::handleCollisions(const std::vector<std::vector<int>>& levelData, const std::vector<CollisionType>& collisionData, int tileWidth, int tileHeight) {
    sf::FloatRect hitbox = getHitbox();

    for (size_t tileY = 0; tileY < levelData.size(); ++tileY) {
        for (size_t tileX = 0; tileX < levelData[tileY].size(); ++tileX) {
            int tileIndex = levelData[tileY][tileX];
            if (tileIndex >= 0 && tileIndex < collisionData.size()) {
                CollisionType collisionType = collisionData[tileIndex];
                if (collisionType == CollisionType::None) {
                    continue;
                }

                sf::FloatRect tileRect(tileX * tileWidth, tileY * tileHeight, tileWidth, tileHeight);

                if (hitbox.intersects(tileRect)) {
                    if (collisionType == CollisionType::Solid) {
                        // Check bottom part of hitbox for landing on tiles
                        if (velocityY > 0 && hitbox.top + hitbox.height <= tileRect.top + velocityY) {
                            y = tileRect.top - hitbox.height;
                            velocityY = 0;
                            jumpCount = 0;
                            isAirborne = false;
                        }
                        // Check top part of hitbox for hitting the bottom of tiles
                        if (velocityY < 0 && hitbox.top >= tileRect.top + tileRect.height - velocityY) {
                            y = tileRect.top + tileRect.height;
                            velocityY = 0;
                        }
                        // Check right part of hitbox for hitting the left side of tiles
                        if (velocityX > 0 && hitbox.left + hitbox.width <= tileRect.left + velocityX) {
                            x = tileRect.left - hitbox.width;
                            velocityX = 0;
                        }
                        // Check left part of hitbox for hitting the right side of tiles
                        if (velocityX < 0 && hitbox.left >= tileRect.left + tileRect.width - velocityX) {
                            x = tileRect.left + tileRect.width;
                            velocityX = 0;
                        }
                    } else if (collisionType == CollisionType::Platform) {
                        // Check bottom part of hitbox for landing on platforms
                        if (velocityY > 0 && hitbox.top + hitbox.height <= tileRect.top + velocityY) {
                            y = tileRect.top - hitbox.height;
                            velocityY = 0;
                            jumpCount = 0;
                            isAirborne = false;
                        }
                    }
                }
            }
        }
    }

    // Verifica daca caracterul atinge josul ecranului
    if (hitbox.top + hitbox.height >= 700) {
        y = 700 - hitbox.height;
        velocityY = 0.0f;
        jumpCount = 0;
        isAirborne = false;
    }
}

void Character::applyGravity(float deltaTime) {
    velocityY += gravity * deltaTime;
    if (velocityY > maxFallSpeed) {
        velocityY = maxFallSpeed;
    }
}

std::ostream& operator<<(std::ostream& os, const Character& character) {
    os << "Character(Name: " << character.name << ", Health: " << character.health << ", Position: (" << character.x << ", " << character.y << "))";
    return os;
}