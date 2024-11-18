#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "CollisionType.h"

class Level {
public:
    Level(const std::string& backgroundFile);
    void loadTileset(const std::string& tilesetFile, int tileWidth, int tileHeight);
    void setLevelData(const std::vector<std::vector<int>>& data);
    void render(sf::RenderWindow& window);
    void updateBackgroundScale(const sf::RenderWindow& window);
    bool isTileTransparent(const sf::Sprite& tile);

    void saveLevelData(const std::string& filename);
    void loadLevelData(const std::string& filename);

    void showTilesetWithGrid();

    double getTileScale() const;
    std::vector<std::vector<int>> getLevelData() const;
    std::vector<CollisionType> getCollisionData() const;
    int getTileWidth() const;
    int getTileHeight() const;
    std::vector<sf::Sprite> getTiles() const;

    friend std::ostream& operator<<(std::ostream& os, const Level& level);

private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture tilesetTexture;
    std::vector<sf::Sprite> tiles;
    std::vector<CollisionType> collisionData;
    std::vector<std::vector<int>> levelData;
    int tileWidth;
    int tileHeight;
    double backgroundScale;
    double tileScale;
};

#endif // LEVEL_H