#include "../include/LevelEditor.h"
// #include "../include/PropsManager.h"

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

void LevelEditor::handleInput(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    int gridX = mousePos.x / (tileWidth * tileScale);
    int gridY = mousePos.y / (tileHeight * tileScale);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (gridX >= 0 && gridX < (int)levelData[0].size() && gridY >= 0 && gridY < (int)levelData.size()) {
            levelData[gridY][gridX] = currentTileIndex;
        }
    } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        if (gridX >= 0 && gridX < (int)levelData[0].size() && gridY >= 0 && gridY < (int)levelData.size()) {
            levelData[gridY][gridX] = -1;
        }
    }

    // If CTRL + S is pressed, save the level data to a file
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        level.setLevelData(levelData);
        level.saveLevelData("level.txt");
    }
}

void LevelEditor::render(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    int gridX = mousePos.x / (tileWidth * tileScale);
    int gridY = mousePos.y / (tileHeight * tileScale);

    level.setLevelData(levelData);
    level.render(window);
    
    // Draw the current tile sprite at the mouse position over the level
    if (gridX >= 0 && gridX < (int)levelData[0].size() && gridY >= 0 && gridY < (int)levelData.size()) {
        currentTileSprite.setPosition(gridX * tileWidth * tileScale, gridY * tileHeight * tileScale);
        window.draw(currentTileSprite);
    }
    // drawGrid(window);
}

// void LevelEditor::drawGrid(sf::RenderWindow& window) {
//     for (int y = 0; y < levelData.size(); ++y) {
//         for (int x = 0; x < levelData[y].size(); ++x) {
//             gridCell.setPosition(x * tileWidth * tileScale, y * tileHeight * tileScale);
//             window.draw(gridCell);
//         }
//     }
// }

void LevelEditor::updateLevelData() {
    levelData = level.getLevelData();
}

void LevelEditor::updateCurrentTileSprite() {
    if (currentTileIndex < level.getTiles().size()) {
        currentTileSprite = level.getTiles()[currentTileIndex];
        currentTileSprite.setScale(tileScale, tileScale);
    }
}