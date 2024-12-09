#include "../include/Level.h"
#include "../include/Exception.h"

#include <iostream>
#include <fstream>
#include <sstream>

Level::Level() : tileWidth(0), tileHeight(0), backgroundScale(3.0), tileScale(3.0) {}

Level::Level(const std::string& backgroundFile) : backgroundScale(3.0), tileScale(3.0) {
    if (!backgroundTexture.loadFromFile(backgroundFile)) {
        // std::cerr << "Error: Failed to load background texture" << std::endl;
        throw FileLoadException("Failed to load background texture in Level::Level");
    }
    backgroundSprite.setTexture(backgroundTexture);
}

void Level::loadTileset(const std::string& tilesetFile, int _tileWidth, int _tileHeight) {
    if (!tilesetTexture.loadFromFile(tilesetFile)) {
        // std::cerr << "Error: Failed to load tileset texture" << std::endl;
        throw FileLoadException("Failed to load tileset texture in Level::loadTileset");
    }

    tileWidth = _tileWidth;
    tileHeight = _tileHeight;

    int columns = tilesetTexture.getSize().x / tileWidth;
    int rows = tilesetTexture.getSize().y / tileHeight;
    int transparentTiles = 0;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < columns; ++x) {
            sf::Sprite tile;
            tile.setTexture(tilesetTexture);
            tile.setTextureRect(sf::IntRect(x * tileWidth, y * tileHeight, tileWidth, tileHeight));

            // Check if the tile is empty / transparent
            if (!isTileTransparent(tile)) 
                tiles.push_back(tile);
            else
                transparentTiles++;
        }
    }

    // Might be wrong, but this is the collision data for the tileset
    std::string collisionDataString = "11112210110011011111111111122111111111221111001100111111111121111111111100022211110111111111111011111111111111111111111111111111";

    // Convert the string to an array of CollisionType
    collisionData.clear();
    for (char c : collisionDataString) {
        switch (c) {
            case '0':
                collisionData.push_back(CollisionType::None);
                break;
            case '1':
                collisionData.push_back(CollisionType::Solid);
                break;
            case '2':
                collisionData.push_back(CollisionType::Platform);
                break;
            default:
                // collisionData.push_back(CollisionType::None);
                throw InvalidOperationException("Invalid collision data in Level::loadTileset");
                break;
        }
    }

    std::cout << "Loaded " << tiles.size() << " tiles\n";
    std::cout << "Found " << transparentTiles << " transparent tiles\n";

    // DEBUG
    // showTilesetWithGrid();
}

bool Level::isTileTransparent(const sf::Sprite& tile) {
    sf::Texture tileTexture = *tile.getTexture();
    sf::Image tileImage = tileTexture.copyToImage();
    sf::IntRect rect = tile.getTextureRect();

    for (int y = 0; y < rect.height; ++y) {
        for (int x = 0; x < rect.width; ++x) {
            if (tileImage.getPixel(rect.left + x, rect.top + y).a != 0) {
                return false;
            }
        }
    }
    return true;
}

// void Level::showTilesetWithGrid() {
//     sf::RenderWindow tilesetWindow(sf::VideoMode(tilesetTexture.getSize().x, tilesetTexture.getSize().y), "Tileset Grid");

//     while (tilesetWindow.isOpen()) {
//         sf::Event event;
//         while (tilesetWindow.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) {
//                 tilesetWindow.close();
//             }
//         }

//         tilesetWindow.clear();

//         for (size_t i = 0; i < tiles.size(); ++i) {
//             sf::Sprite& tile = tiles[i];
//             int x = (i % (tilesetTexture.getSize().x / tileWidth)) * tileWidth;
//             int y = (i / (tilesetTexture.getSize().x / tileWidth)) * tileHeight;
//             tile.setPosition(x, y);
//             tilesetWindow.draw(tile);

//             // Draw grid lines
//             sf::RectangleShape line(sf::Vector2f(tileWidth, 1));
//             line.setFillColor(sf::Color::Red);
//             line.setPosition(x, y);
//             tilesetWindow.draw(line);

//             line.setSize(sf::Vector2f(1, tileHeight));
//             line.setPosition(x, y);
//             tilesetWindow.draw(line);
//         }

//         tilesetWindow.display();
//     }
// }

void Level::setLevelData(const std::vector<std::vector<int>>& data) {
    levelData = data;
}

const std::vector<std::vector<int>>& Level::getLevelData() const {
    return levelData;
}

const std::vector<sf::Sprite>& Level::getTiles() const {
    return tiles;
}

void Level::render(sf::RenderWindow& window) {
    sf::Vector2f viewCenter = window.getView().getCenter();
    sf::Vector2f viewSize = window.getView().getSize();
    sf::FloatRect viewRect(viewCenter - viewSize / 2.0f, viewSize);

    // Calculate parallax offset
    float parallaxFactor = 0.5f; // Adjust this value to control the parallax effect
    float parallaxOffsetX = viewCenter.x * parallaxFactor;

    backgroundSprite.setScale(backgroundScale, backgroundScale);

    // Draw background tiles with parallax effect
    for (float x = viewRect.left - parallaxOffsetX; x < viewRect.left + viewRect.width; x += backgroundTexture.getSize().x * backgroundScale) {
        backgroundSprite.setPosition(x, 0);
        window.draw(backgroundSprite);
    }

    // Draw level tiles
    for (size_t y = 0; y < levelData.size(); ++y) {
        for (size_t x = 0; x < levelData[y].size(); ++x) {
            int tileIndex = levelData[y][x];
            if (tileIndex >= 0 && tileIndex < (int)tiles.size()) {
                sf::Sprite& tile = tiles[tileIndex];
                tile.setScale(tileScale, tileScale);
                tile.setPosition(x * tileWidth * tileScale, y * tileHeight * tileScale);
                window.draw(tile);
            }
        }
    }
}

void Level::updateBackgroundScale(const sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();

    // DEBUG
    std::cout << "windowSize: " << windowSize.x << ", " << windowSize.y << '\n';
    std::cout << "textureSize: " << textureSize.x << ", " << textureSize.y << '\n';
    std::cout << "scale = " << windowSize.y << " / " << textureSize.y << '\n';
    
    backgroundScale = windowSize.y / textureSize.y;

    std::cout << "scale: " << backgroundScale << '\n';

    backgroundSprite.setScale(backgroundScale, backgroundScale);
}

void Level::expandLevel(int additionalColumns) {
    for (auto& row : levelData) {
        row.insert(row.end(), additionalColumns, -1); // Add empty tiles
    }

    // Add solid tiles to the bottom of the level
    int lastRowIndex = levelData.size() - 1;
    for (int i = 0; i < additionalColumns; ++i) {
        levelData[lastRowIndex][levelData[lastRowIndex].size() - additionalColumns + i] = 24;
    }
}

double Level::getTileScale() const {
    return tileScale;
}

void Level::saveLevelData(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        // std::cerr << "Error: Failed to open file for writing\n";
        throw FileLoadException("Failed to open file for writing in Level::saveLevelData");
    }

    for (size_t y = 0; y < levelData.size(); ++y) {
        for (size_t x = 0; x < levelData[y].size(); ++x) {
            file << levelData[y][x] << ' ';
        }
        file << '\n';
    }

    file.close();
}

void Level::loadLevelData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // std::cerr << "Error: Failed to open file for reading\n";
        throw FileLoadException("Failed to open file for reading in Level::loadLevelData");
    }

    std::vector<std::vector<int>> data;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        int value;
        while (ss >> value) {
            row.push_back(value);
        }
        data.push_back(row);
    }

    levelData = data;
}

const std::vector<CollisionType>& Level::getCollisionData() const {
    return collisionData;
}

int Level::getTileWidth() const {
    return tileWidth * tileScale;
}

int Level::getTileHeight() const {
    return tileHeight * tileScale;
}

std::ostream& operator<<(std::ostream& os, const Level& level) {
    os << "Level data:\n";
    for (size_t y = 0; y < level.levelData.size(); ++y) {
        for (size_t x = 0; x < level.levelData[y].size(); ++x) {
            os << level.levelData[y][x] << ' ';
        }
        os << '\n';
    }
    return os;
}