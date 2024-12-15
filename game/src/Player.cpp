#include "../include/Player.h"
#include "../include/Exception.h"

Player::Player(const std::string& name, int health, int x, int y, float scale)
    : Entity(name, x, y), health(health), state(State::Idle), scale(scale), facingRight(true), isAirborne(true), currentFrame(0), frameTime(0.1f), elapsedTime(0.0f), lastAttackTime(0.0f),
      velocityX(0.0f), velocityY(0.0f), gravity(1200.0f), jumpCount(0), maxJumps(2), maxFallSpeed(800.0f), lastJumpTime(std::chrono::steady_clock::now()) {
    loadAnimation("assets/player/sunny-bunny-idle.png", 4, idleSprites, idleTexture);
    loadAnimation("assets/player/sunny-bunny-jump.png", 5, jumpSprites, jumpTexture);
    loadAnimation("assets/player/sunny-bunny-run.png", 6, runSprites, runTexture);
    loadAnimation("assets/player/vfx/slash-horizontal.png", 5, attackSprites, attackTexture);

    if (!attackTexture.loadFromFile("assets/player/vfx/slash-horizontal.png")) {
        throw std::runtime_error("Failed to load slash texture");
    }
    slashSprite.setTexture(attackTexture);
    slashSprite.setScale(scale, scale);
}

Player::Player(const Player& other)
    : Entity(other), health(other.health), state(other.state), scale(other.scale), facingRight(other.facingRight), isAirborne(other.isAirborne),
      idleSprites(other.idleSprites), jumpSprites(other.jumpSprites), runSprites(other.runSprites), attackSprites(other.attackSprites), slashSprite(other.slashSprite), currentFrame(other.currentFrame),
      frameTime(other.frameTime), elapsedTime(other.elapsedTime), lastAttackTime(other.lastAttackTime), velocityX(other.velocityX), velocityY(other.velocityY), gravity(other.gravity),
      jumpCount(other.jumpCount), maxJumps(other.maxJumps), maxFallSpeed(other.maxFallSpeed), lastJumpTime(other.lastJumpTime) {}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        Entity::operator=(other);
        health = other.health;
        state = other.state;
        scale = other.scale;
        facingRight = other.facingRight;
        isAirborne = other.isAirborne;
        currentFrame = other.currentFrame;
        frameTime = other.frameTime;
        elapsedTime = other.elapsedTime;
        lastAttackTime = other.lastAttackTime;
        velocityX = other.velocityX;
        velocityY = other.velocityY;
        gravity = other.gravity;
        jumpCount = other.jumpCount;
        maxJumps = other.maxJumps;
        maxFallSpeed = other.maxFallSpeed;
        lastJumpTime = other.lastJumpTime;
        idleSprites = other.idleSprites;
        jumpSprites = other.jumpSprites;
        runSprites = other.runSprites;
        attackSprites = other.attackSprites;
        slashSprite = other.slashSprite;
    }
    return *this;
}

//Entity* Player::clone() const {
//return new Player(*this);
//}

Player::~Player() {
    idleSprites.clear();
    jumpSprites.clear();
    runSprites.clear();
    attackSprites.clear();
}

const sf::FloatRect Player::getHitbox() const {
    return sf::FloatRect(x, y, idleSprites[0].getGlobalBounds().width, idleSprites[0].getGlobalBounds().height);
}

void Player::jump() {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedJumpTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastJumpTime).count();

    if (elapsedJumpTime < 500) {
        return;
    }

    if (jumpCount < maxJumps) {
        y -= 3.0f; // Move up slightly to prevent collision with the ground
        velocityY = -800.0f;
        jumpCount++;
        lastJumpTime = currentTime;
        isAirborne = true;
    }
}

void Player::move(float dx, float dy) {
    velocityX = dx;
    velocityY += dy;

    if (dx > 0) {
        facingRight = true;
    } else if (dx < 0) {
        facingRight = false;
    }
}

// void Player::takeDamage(int amount) {
//     health -= amount;
//     if (health < 0) health = 0;
// }

// void Player::heal(int amount) {
//     health += amount;
// }

void Player::update(float _deltaTime) {
    deltaTime = _deltaTime;
}

void Player::attack() {
    if (state != State::Attacking && (lastAttackTime * 1000) > 300) {
        lastAttackTime = 0.0f;
        state = State::Attacking;
        currentFrame = 0;
        elapsedTime = 0.0f;
    }
}

void Player::update(float _deltaTime, const std::vector<std::vector<int>>& levelData, const std::vector<CollisionType>& collisionData, int tileWidth, int tileHeight) {
    State oldState = state;

    if (state != State::Attacking) {
        if (isAirborne) {
            state = State::Jumping;
        } else {
            if (velocityX == 0) {
                state = State::Idle;
            } else {
                state = State::Running;
            }
        }
    }

    if (oldState != state) {
        currentFrame = 0;
    }

    deltaTime = _deltaTime;

    handleCollisions(levelData, collisionData, tileWidth, tileHeight);

    applyGravity(deltaTime);

    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    updateAnimation(deltaTime);
}

// void Player::drawHitbox(sf::RenderWindow& window) {
//     sf::RectangleShape rect;
//     sf::FloatRect hitbox = getHitbox();
//     rect.setSize(sf::Vector2f(hitbox.width, hitbox.height));
//     rect.setPosition(hitbox.left, hitbox.top);
//     rect.setFillColor(sf::Color::Transparent);
//     rect.setOutlineColor(sf::Color::Red);
//     rect.setOutlineThickness(1.0f);

//     window.draw(rect);
// }

void Player::render(sf::RenderWindow& window) {
    sf::Sprite* currentSprite = nullptr;

    switch (state) {
        case State::Idle:
            if (currentFrame >= (int)idleSprites.size()) currentFrame = 0;
            currentSprite = &idleSprites[currentFrame];
            break;
        case State::Running:
            if (currentFrame >= (int)runSprites.size()) currentFrame = 0;
            currentSprite = &runSprites[currentFrame];
            break;
        case State::Jumping:
            if (currentFrame >= (int)jumpSprites.size()) currentFrame = 0;
            currentSprite = &jumpSprites[currentFrame];
            break;
        case State::Attacking:
            // currentSprite = &attackSprites[currentFrame];
            if (velocityX == 0) {
                currentSprite = &idleSprites[currentFrame];
            } else {
                currentSprite = &runSprites[currentFrame];
            }
            break;
    }

    // DEBUG
    // drawHitbox(window);

    // window.draw(rect1);
    // window.draw(rect2);

    if (currentSprite) {
        currentSprite->setPosition(x, y);
        if (facingRight) {
            currentSprite->setScale(scale, scale);
        } else {
            currentSprite->setScale(-scale, scale);
            currentSprite->setPosition(x + currentSprite->getGlobalBounds().width, y);
        }
        window.draw(*currentSprite);
    }

    // Render slashing animation separately
    if (state == State::Attacking) {
        int frameIndex = static_cast<int>((lastAttackTime * 1000) / 30) % 5; // 30ms per frame

        if (facingRight) {
            attackSprites[frameIndex].setPosition(x + 10.0f, y);
            attackSprites[frameIndex].setScale(scale, scale);
        } else {
            attackSprites[frameIndex].setPosition(x - 10.0f + currentSprite->getGlobalBounds().width, y);
            attackSprites[frameIndex].setScale(-scale, scale); // Flip horizontally
        }

        if (frameIndex >= 4) {
            window.draw(attackSprites[frameIndex]);
            state = State::Idle; // Reset to idle after attack animation
        } else {
            window.draw(attackSprites[frameIndex]);
        }
    }
}

void Player::loadAnimation(const std::string& filePath, int frameCount, std::vector<sf::Sprite>& sprites, sf::Texture& _texture) {
    if (!_texture.loadFromFile(filePath)) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return;
    }

    int frameWidth = _texture.getSize().x / frameCount;
    int frameHeight = _texture.getSize().y;

    for (int i = 0; i < frameCount; ++i) {
        sf::Sprite _sprite;
        _sprite.setTexture(_texture);
        _sprite.setTextureRect(sf::IntRect(i * frameWidth, 0, frameWidth, frameHeight));
        _sprite.setScale(scale, scale);
        sprites.push_back(_sprite);
    }
}

void Player::updateAnimation(float _deltaTime) {
    elapsedTime += _deltaTime;
    lastAttackTime += _deltaTime;
    if (elapsedTime >= frameTime) {
        elapsedTime = 0.0f;

        // DEBUG
        // std::cout << "State: " << (int)state << ", Frame: " << currentFrame << '\n';

        switch (state) {
            case State::Idle:
                currentFrame = (currentFrame + 1) % (int)idleSprites.size();
                break;
            case State::Running:
                currentFrame = (currentFrame + 1) % (int)runSprites.size();
                break;
            case State::Jumping:
                currentFrame = (currentFrame + 1) % (int)jumpSprites.size();
                if (velocityY < -100.0f) {
                    currentFrame = 1; // Thrusting upwards
                } else if (velocityY > 100.0f) {
                    currentFrame = 3; // Falling down
                } else {
                    currentFrame = 2; // Midair
                }
                break;
            case State::Attacking:
                break;       
        }
    }
}

void Player::handleCollisions(const std::vector<std::vector<int>>& levelData, const std::vector<CollisionType>& collisionData, int tileWidth, int tileHeight) {
    sf::FloatRect hitbox = getHitbox();
    bool isCollidingWithTile = false;

    sf::FloatRect horizontalHitbox = sf::FloatRect(hitbox.left, hitbox.top + hitbox.height / 6, hitbox.width, hitbox.height * 2 / 3);
    sf::FloatRect verticalHitbox = sf::FloatRect(hitbox.left + hitbox.width / 6, hitbox.top, hitbox.width * 2 / 3, hitbox.height);
    sf::FloatRect feetVerticalHitbox = sf::FloatRect(hitbox.left + hitbox.width / 6, hitbox.top + hitbox.height * 4 / 5, hitbox.width * 2 / 3, hitbox.height / 5);
    
    // Let's check for future collisions meaning we add the velocity to the hitbox
    horizontalHitbox.left += velocityX * deltaTime;
    verticalHitbox.top += velocityY * deltaTime;
    feetVerticalHitbox.top += velocityY * deltaTime;

    for (size_t tileY = 0; tileY < levelData.size(); ++tileY) {
        for (size_t tileX = 0; tileX < levelData[tileY].size(); ++tileX) {
            int tileIndex = levelData[tileY][tileX];
            if (tileIndex >= 0 && tileIndex < (int)collisionData.size()) {
                CollisionType collisionType = collisionData[tileIndex];
                if (collisionType == CollisionType::None) {
                    continue;
                }

                sf::FloatRect tileRect(tileX * tileWidth, tileY * tileHeight, tileWidth, tileHeight);

                if (horizontalHitbox.intersects(tileRect)) {
                    isCollidingWithTile = true;

                    // DEBUG
                    rect1.setPosition(horizontalHitbox.left, horizontalHitbox.top);
                    rect1.setSize(sf::Vector2f(horizontalHitbox.width, horizontalHitbox.height));
                    rect1.setFillColor(sf::Color::Transparent);
                    rect1.setOutlineColor(sf::Color::Red);
                    rect1.setOutlineThickness(3.0f);

                    rect2.setPosition(tileRect.left, tileRect.top);
                    rect2.setSize(sf::Vector2f(tileRect.width, tileRect.height));
                    rect2.setFillColor(sf::Color::Transparent);
                    rect2.setOutlineColor(sf::Color::Magenta);
                    rect2.setOutlineThickness(3.0f);
                    
                    if (collisionType == CollisionType::Solid) {
                        // Check left part of horizontal hitbox for hitting the right side of tiles
                        if (velocityX < 0 && horizontalHitbox.left + horizontalHitbox.width >= tileRect.left + velocityX) {
                            velocityX = 0;
                        }
                        // Check right part of horizontal hitbox for hitting the left side of tiles
                        else if (velocityX > 0 && horizontalHitbox.left <= tileRect.left + tileRect.width + velocityX) {
                            velocityX = 0;
                        }
                    }
                } 
                else if (verticalHitbox.intersects(tileRect)) {

                    // DEBUG
                    rect1.setPosition(verticalHitbox.left, verticalHitbox.top);
                    rect1.setSize(sf::Vector2f(verticalHitbox.width, verticalHitbox.height));
                    rect1.setFillColor(sf::Color::Transparent);
                    rect1.setOutlineColor(sf::Color::Red);
                    rect1.setOutlineThickness(3.0f);
                    
                    rect2.setPosition(tileRect.left, tileRect.top);
                    rect2.setSize(sf::Vector2f(tileRect.width, tileRect.height));
                    rect2.setFillColor(sf::Color::Transparent);
                    rect2.setOutlineColor(sf::Color::Magenta);
                    rect2.setOutlineThickness(3.0f);

                    if (collisionType == CollisionType::Solid) {
                        // Check bottom part of vertical hitbox for landing on tiles
                        if (velocityY > 0 && verticalHitbox.top + verticalHitbox.height <= tileRect.top + velocityY) {
                            y = tileRect.top - hitbox.height;
                            velocityY = 0.0f;
                            jumpCount = 0;
                            isAirborne = false;
                        }
                        // Check top part of vertical hitbox for hitting the bottom of tiles
                        else if (velocityY < 0 && verticalHitbox.top >= tileRect.top + tileRect.height + velocityY) {
                            // y += 3.0f;
                            velocityY = 100.0f;
                        }
                    }
                }
                
                if (feetVerticalHitbox.intersects(tileRect)) {

                    // DEBUG
                    rect1.setPosition(feetVerticalHitbox.left, feetVerticalHitbox.top);
                    rect1.setSize(sf::Vector2f(feetVerticalHitbox.width, feetVerticalHitbox.height));
                    rect1.setFillColor(sf::Color::Transparent);
                    rect1.setOutlineColor(sf::Color::Red);
                    rect1.setOutlineThickness(3.0f);

                    rect2.setPosition(tileRect.left, tileRect.top);
                    rect2.setSize(sf::Vector2f(tileRect.width, tileRect.height));
                    rect2.setFillColor(sf::Color::Transparent);
                    rect2.setOutlineColor(sf::Color::Magenta);
                    rect2.setOutlineThickness(3.0f);

                    if (collisionType == CollisionType::Platform) {
                        // Check bottom part of vertical hitbox for landing on platforms
                        if (velocityY > 0 && feetVerticalHitbox.top + feetVerticalHitbox.height <= tileRect.top + velocityY) {
                            y = tileRect.top - hitbox.height;
                            velocityY = 0.0f;
                            jumpCount = 0;
                            isAirborne = false;
                        }
                    }
                }
            }
        }
    }

    // Check if the character is touching the bottom of the screen
    if (hitbox.top + hitbox.height >= 700) {
        y = 0;
        
        velocityY = 0.0f;
        jumpCount = 0;
        isAirborne = false;
    }

    // Check if the character is not colliding with any tiles and is not touching the bottom of the screen
    // Whilst not airborne, accelerating downwards with a velocity greater than the last velocity and greater than 200.0f
    else if (!isCollidingWithTile && !isAirborne && velocityY > 20.0f) {
        isAirborne = true;
    }

    // Check if player is out of bounds
    if (x < -200.0f) throw PlayerOutOfBoundsException("Player is out of bounds to the left");
    else if (x > 8000.0f) throw PlayerOutOfBoundsException("Player is out of bounds to the right");

    // lastVelocityY = velocityY;
}

const sf::Vector2f Player::getPosition() const {
    return sf::Vector2f(x, y);
}

// const sf::Vector2f Player::getVelocity() const {
//     return sf::Vector2f(velocityX, velocityY);
// }

void Player::applyGravity(float _deltaTime) {
    velocityY += gravity * _deltaTime;
    if (velocityY > maxFallSpeed) {
        velocityY = maxFallSpeed;
    }
}

std::ostream& operator<<(std::ostream& os, const Player& character) {
    os << "Character(Name: " << character.name << ", Health: " << character.health << ", Position: (" << character.x << ", " << character.y << "))";
    return os;
}