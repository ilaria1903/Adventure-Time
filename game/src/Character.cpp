#include "../include/Character.h"

Character::Character(const std::string& name, int health, float x, float y, float speed)
    : name(name), health(health), x(x), y(y), speed(speed) {}

Character::Character(const Character& other)
    : name(other.name), health(other.health), x(other.x), y(other.y), speed(other.speed) {}

Character& Character::operator=(const Character& other) {
    if (this != &other) {
        name = other.name;
        health = other.health;
        x = other.x;
        y = other.y;
        speed = other.speed;
    }
    return *this;
}

Character::~Character() {}

void Character::move(int dx, int dy) {
    x += dx * speed;
    y += dy * speed;
}

void Character::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void Character::heal(int amount) {
    health += amount;
}

std::ostream& operator<<(std::ostream& os, const Character& character) {
    os << "Character(" << character.name << ", Health: " << character.health << ", Position: (" << character.x << ", " << character.y << "))";
    return os;
}