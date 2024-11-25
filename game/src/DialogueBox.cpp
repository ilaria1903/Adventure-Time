#include "../include/DialogueBox.h"

DialogueBox::DialogueBox() : visible(false), message(""), timeSinceLastChar(0.0f), charDisplayInterval(0.05f), currentCharIndex(0) {
    if (!font.loadFromFile("assets/fonts/retro_gaming.ttf")) {
        throw std::runtime_error("Failed to load font");
    }
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(50, 150);

    background.setSize(sf::Vector2f(700, 100));
    background.setFillColor(sf::Color(0, 0, 0, 150));
    background.setPosition(50, 150);
}

void DialogueBox::hide() {
    visible = false;
}

void DialogueBox::show(const std::string& msg) {
    message = msg;
    currentCharIndex = 0;
    text.setString("");
    timeSinceLastChar = 0.0f;
    visible = true;
}

void DialogueBox::render(sf::RenderWindow& window) {
    if (visible) {
        window.draw(background);
        window.draw(text);
    }
}

void DialogueBox::update(float deltaTime) {
    if (visible && currentCharIndex < message.length()) {
        timeSinceLastChar += deltaTime;
        if (timeSinceLastChar >= charDisplayInterval) {
            currentCharIndex++;
            text.setString(message.substr(0, currentCharIndex));
            timeSinceLastChar = 0.0f;
        }
    }
}