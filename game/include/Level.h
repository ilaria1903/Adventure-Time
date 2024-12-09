#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "CollisionType.h"

class Level {
public:
    Level();
    explicit Level(const std::string& backgroundFile);
    void loadTileset(const std::string& tilesetFile, int tileWidth, int tileHeight);
    void setLevelData(const std::vector<std::vector<int>>& data);
    void render(sf::RenderWindow& window);
    void updateBackgroundScale(const sf::RenderWindow& window);
    bool isTileTransparent(const sf::Sprite& tile);

    void saveLevelData(const std::string& filename);
    void loadLevelData(const std::string& filename);
    void expandLevel(int additionalColumns);

    // void showTilesetWithGrid();

    double getTileScale() const;
    const std::vector<std::vector<int>>& getLevelData() const;
    const std::vector<CollisionType>& getCollisionData() const;
    const std::vector<sf::Sprite>& getTiles() const;
    int getTileWidth() const;
    int getTileHeight() const;

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