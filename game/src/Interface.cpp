#include "../include/Interface.h"
#include "../include/Exception.h"

Interface::Interface() : cherryCount(0), playerHealth(5) {
    if (!font.loadFromFile("assets/fonts/retro_gaming.ttf")) {
        throw InvalidOperationException("Failed to load font retro_gaming.ttf in Interface constructor");
    }
    if (!cherryTexture.loadFromFile("assets/props/cherry.png")) {
        throw InvalidOperationException("Failed to load cherry texture in Interface constructor");
    }
    if (!heartTexture.loadFromFile("assets/props/heart_spritesheet.png")) {
        throw InvalidOperationException("Failed to load heart sprite sheet in Interface constructor");
    }

    cherrySprite.setTexture(cherryTexture);
    cherrySprite.setScale(3.0f, 3.0f);
    cherrySprite.setPosition(10, 10);

    cherryText.setFont(font);
    cherryText.setCharacterSize(24);
    cherryText.setFillColor(sf::Color::White);
    cherryText.setOutlineColor(sf::Color::Black);
    cherryText.setOutlineThickness(2);
    cherryText.setPosition(70, 30);
    updateCherries(cherryCount);
}

void Interface::setWindowSize(const sf::Vector2u& size) {
    windowSize = size;
    for (int i = 0; i < 5; ++i) {
        sf::Sprite heartSprite;
        heartSprite.setTexture(heartTexture);
        heartSprite.setScale(2.0f, 2.0f);
        heartSprite.setPosition(windowSize.x - 60 - i * 35, 20);
        heartSprites.push_back(heartSprite);
    }
    updateHearts(playerHealth);
}

void Interface::updateCherries(int count) {
    cherryCount = count;
    cherryText.setString("x " + std::to_string(cherryCount));
}

void Interface::updateHearts(int health) {
    playerHealth = health;
    for (int i = 0; i < 5; ++i) {
        if (i < playerHealth) {
            heartSprites[i].setTextureRect(sf::IntRect(0, 0, 17, 17)); // Full heart
        } else {
            heartSprites[i].setTextureRect(sf::IntRect(68, 0, 17, 17)); // Missing heart
        }
    }
}

void Interface::render(sf::RenderWindow& window) {
    window.draw(cherrySprite);
    window.draw(cherryText);
    for (const auto& heartSprite : heartSprites) {
        window.draw(heartSprite);
    }
}