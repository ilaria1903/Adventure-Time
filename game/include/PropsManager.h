#ifndef PROPSMANAGER_H
#define PROPSMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>

struct PropInstance {
    sf::Sprite sprite;
    std::string name;
    int x, y;
    float scale;
};

class PropsManager {
public:
    void loadProp(const std::string& name, const std::string& filePath);
    void addPropInstance(const std::string& name, float scale = 3.0f, int x = 0, int y = 0);
    void removePropInstance(float x = 0, float y = 0);
    void renderProps(sf::RenderWindow& window);
    void setWindow(sf::RenderWindow& window);
    void setTileSize(float width, float height, float scale);
    std::vector<PropInstance>& getPropInstances();

private:
    float tileWidth, tileHeight, tileScale;
    sf::RenderWindow* window;
    std::unordered_map<std::string, sf::Texture> textures;
    sf::Clock lastPlaced;
    std::vector<PropInstance> propInstances;
};

#endif // PROPSMANAGER_H