#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include <SFML/Graphics.hpp>
#include "Level.h"

class LevelEditor {
public:
    LevelEditor(Level& level, int tileWidth, int tileHeight, float tileScale);
    LevelEditor(const LevelEditor& other) = default;
    void handleInput(sf::RenderWindow& window);
    void handleMouseScroll(const sf::Event::MouseWheelScrollEvent& scrollEvent);
    void render(sf::RenderWindow& window, const sf::Vector2f& _playerPos);
    void updateLevelData();

    int getTileIndex(float x, float y) const;

private:
    Level& level;
    int tileWidth;
    int tileHeight;
    float tileScale;
    size_t currentTileIndex;
    std::vector<std::vector<int>> levelData;
    std::vector<sf::Sprite> tiles;
    sf::RectangleShape gridCell;
    sf::Sprite currentTileSprite;

    // void drawGrid(sf::RenderWindow& window);
    void updateCurrentTileSprite();
};

#endif // LEVELEDITOR_H