#ifndef DIALOGUEBOX_H
#define DIALOGUEBOX_H

#include <SFML/Graphics.hpp>
#include <string>

class DialogueBox {
public:
    DialogueBox();
    void show(const std::string& msg);
    void update(float deltaTime, const sf::View& cameraView);
    void hide();
    void render(sf::RenderWindow& window);

    bool isVisible() const { return visible; }

private:
    sf::Font font;
    sf::Text text;
    sf::RectangleShape background;
    bool visible;
    std::string message;
    float timeSinceLastChar;
    float charDisplayInterval;
    size_t currentCharIndex;
};

#endif // DIALOGUEBOX_H