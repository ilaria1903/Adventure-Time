#include "../include/Level.h"

#include <iostream>
#include <fstream>
#include <sstream>

// Constructor for Level - loads the background texture
Level::Level(const std::string& backgroundFile) : tileScale(3.0), backgroundScale(3.0) {
    if (!backgroundTexture.loadFromFile(backgroundFile)) {
        std::cerr << "Error: Failed to load background texture" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
}

// Load the tileset and create tiles with a specific width and height
void Level::loadTileset(const std::string& tilesetFile, int tileWidth, int tileHeight) {
    if (!tilesetTexture.loadFromFile(tilesetFile)) {
        std::cerr << "Error: Failed to load tileset texture" << std::endl;
    }

    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;

    int columns = tilesetTexture.getSize().x / tileWidth;
    int rows = tilesetTexture.getSize().y / tileHeight;
    int transparentTiles = 0;

    // Generate individual tile sprites from the tileset
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < columns; ++x) {
            sf::Sprite tile;
            tile.setTexture(tilesetTexture);
            tile.setTextureRect(sf::IntRect(x * tileWidth, y * tileHeight, tileWidth, tileHeight));

            // Check if the tile is empty/transparent
            if (!isTileTransparent(tile))
                tiles.push_back(tile);
            else
                transparentTiles++;
        }
    }

    // Sample collision data; modify as needed
    std::string collisionDataString = "1111221111111111111111111112211111111122111100110011111111111120000111111100022211110111111111111111111111111111111111111111111111";

    // Convert string-based collision data into CollisionType vector
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
                collisionData.push_back(CollisionType::None);
                break;
        }
    }

    std::cout << "Loaded " << tiles.size() << " tiles\n";
    std::cout << "Found " << transparentTiles << " transparent tiles\n";
}

// Check if a tile is transparent (used during tile loading)
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

// Display the tileset with a grid overlay
void Level::showTilesetWithGrid() {
    sf::RenderWindow tilesetWindow(sf::VideoMode(tilesetTexture.getSize().x, tilesetTexture.getSize().y), "Tileset Grid");

    while (tilesetWindow.isOpen()) {
        sf::Event event;
        while (tilesetWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                tilesetWindow.close();
            }
        }

        tilesetWindow.clear();

        for (size_t i = 0; i < tiles.size(); ++i) {
            sf::Sprite& tile = tiles[i];
            int x = (i % (tilesetTexture.getSize().x / tileWidth)) * tileWidth;
            int y = (i / (tilesetTexture.getSize().x / tileWidth)) * tileHeight;
            tile.setPosition(x, y);
            tilesetWindow.draw(tile);

            // Draw grid lines
            sf::RectangleShape line(sf::Vector2f(tileWidth, 1));
            line.setFillColor(sf::Color::Red);
            line.setPosition(x, y);
            tilesetWindow.draw(line);

            line.setSize(sf::Vector2f(1, tileHeight));
            line.setPosition(x, y);
            tilesetWindow.draw(line);
        }

        tilesetWindow.display();
    }
}

// Set the level data (2D grid of tile indices)
void Level::setLevelData(const std::vector<std::vector<int>>& data) {
    levelData = data;
}

// Get the level data (2D grid of tile indices)
std::vector<std::vector<int>> Level::getLevelData() const {
    return levelData;
}

// Get the collection of tile sprites
std::vector<sf::Sprite> Level::getTiles() const {
    return tiles;
}

// Render the background and tiles based on level data
void Level::render(sf::RenderWindow& window) {
    backgroundSprite.setScale(backgroundScale, backgroundScale);
    window.draw(backgroundSprite);

    for (size_t y = 0; y < levelData.size(); ++y) {
        for (size_t x = 0; x < levelData[y].size(); ++x) {
            int tileIndex = levelData[y][x];
            if (tileIndex >= 0 && tileIndex < tiles.size()) {
                sf::Sprite& tile = tiles[tileIndex];
                tile.setScale(tileScale, tileScale);
                tile.setPosition(x * tileWidth * tileScale, y * tileHeight * tileScale);
                window.draw(tile);
            }
        }
    }
}

// Update the background scale to fit the window
void Level::updateBackgroundScale(const sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();

    backgroundScale = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundSprite.setScale(backgroundScale, backgroundScale);
}

// Get the tile scale
double Level::getTileScale() const {
    return tileScale;
}

// Save the level data to a file
void Level::saveLevelData(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file for writing\n";
        return;
    }

    for (size_t y = 0; y < levelData.size(); ++y) {
        for (size_t x = 0; x < levelData[y].size(); ++x) {
            file << levelData[y][x] << ' ';
        }
        file << '\n';
    }

    file.close();
}

// Load the level data from a file
void Level::loadLevelData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file for reading\n";
        return;
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

// Get collision data for each tile
std::vector<CollisionType> Level::getCollisionData() const {
    return collisionData;
}

// Get the width of a tile including scaling
int Level::getTileWidth() const {
    return static_cast<int>(tileWidth * tileScale);
}

// Get the height of a tile including scaling
int Level::getTileHeight() const {
    return static_cast<int>(tileHeight * tileScale);
}

// Overload << operator to print the level data
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
