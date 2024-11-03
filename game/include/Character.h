#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

class Character {
public:
    Character(const std::string& name, int health, float x, float y, float speed = 3.0f);
    Character(const Character& other);
    Character& operator=(const Character& other);
    ~Character();

    int getHealth() const { return health; }
    int getX() const { return x; }
    int getY() const { return y; }

    void move(int dx, int dy);
    void takeDamage(int amount);
    void heal(int amount);

    friend std::ostream& operator<<(std::ostream& os, const Character& character);

private:
    std::string name;
    int health;
    float x, y, speed;
};

#endif // CHARACTER_H