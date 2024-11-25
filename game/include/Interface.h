#ifndef INTERFACE_H
#define INTERFACE_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Interface {
public:
    Interface();
    void updateCherries(int count);
    void render(sf::RenderWindow& window);
    void updateHearts(int health);

    void setWindowSize(const sf::Vector2u& size);

    int getCherryCount() const { return cherryCount; }
    
private:
    sf::Vector2u windowSize;
    int cherryCount;
    sf::Font font;
    sf::Text cherryText;
    sf::Texture cherryTexture;
    sf::Sprite cherrySprite;
    sf::Texture heartTexture;
    std::vector<sf::Sprite> heartSprites;
    int playerHealth;
};

#endif // INTERFACE_H