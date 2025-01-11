#include "../include/LevelEditor.h"

LevelEditor::LevelEditor(Level& level, int tileWidth, int tileHeight, float tileScale)
    : level(level), tileWidth(tileWidth), tileHeight(tileHeight), tileScale(tileScale), currentTileIndex(0) {
    gridCell.setSize(sf::Vector2f(tileWidth * tileScale, tileHeight * tileScale));
    gridCell.setFillColor(sf::Color::Transparent);
    gridCell.setOutlineColor(sf::Color::White);
    gridCell.setOutlineThickness(1.0f);
}

void LevelEditor::handleMouseScroll(const sf::Event::MouseWheelScrollEvent& scrollEvent) {
    if (scrollEvent.delta > 0) {
        currentTileIndex++;
        if (currentTileIndex >= level.getTiles().size()) {
            currentTileIndex = 0;
        }
    } else if (scrollEvent.delta < 0) {
        if (currentTileIndex == 0) {
            currentTileIndex = level.getTiles().size() - 1;
        } else {
            currentTileIndex--;
        }
    }

    updateCurrentTileSprite();
}

void LevelEditor::handleInput(sf::RenderWindow& window, const sf::Vector2f& _playerPos) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    sf::Vector2f adjustedPos = sf::Vector2f(mousePos.x + _playerPos.x - window.getSize().x / 2, mousePos.y);

    int gridX = adjustedPos.x / (tileWidth * tileScale);
    int gridY = adjustedPos.y / (tileHeight * tileScale);

    if (gridX >= 0 && gridY >= 0 && !levelData.empty() && gridY < (int)levelData.size() && gridX < (int)levelData[0].size()) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            levelData[gridY][gridX] = currentTileIndex;
        } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            levelData[gridY][gridX] = -1;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        level.setLevelData(levelData);
        level.saveLevelData("level.txt");
    }
}

void LevelEditor::render(sf::RenderWindow& window, const sf::Vector2f& _playerPos) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f adjustedPos = sf::Vector2f(mousePos.x + _playerPos.x - window.getSize().x / 2, mousePos.y);

    int gridX = adjustedPos.x / (tileWidth * tileScale);
    int gridY = adjustedPos.y / (tileHeight * tileScale);

    level.setLevelData(levelData);
    level.render(window);

    if (gridX >= 0 && gridY >= 0 && !levelData.empty() && gridY < (int)levelData.size() && gridX < (int)levelData[0].size()) {
        currentTileSprite.setPosition(gridX * tileWidth * tileScale, gridY * tileHeight * tileScale);
        window.draw(currentTileSprite);
    }
}

void LevelEditor::updateLevelData() {
    levelData = level.getLevelData();
    if (levelData.empty()) {
        throw std::runtime_error("Level data is empty in updateLevelData.");
    }
}

void LevelEditor::updateCurrentTileSprite() {
    if (!level.getTiles().empty() && currentTileIndex < level.getTiles().size()) {
        currentTileSprite = level.getTiles()[currentTileIndex];
        currentTileSprite.setScale(tileScale, tileScale);
    } else {
        throw std::out_of_range("Current tile index is out of range in updateCurrentTileSprite.");
    }
}
