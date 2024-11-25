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
    static void loadProp(const std::string& name, const std::string& filePath);
    static void addPropInstance(const std::string& name, float scale = 3.0f, int x = 0, int y = 0);
    static void removePropInstance(float x = 0, float y = 0);
    static void renderProps(sf::RenderWindow& window);
    static void setWindow(sf::RenderWindow& window);
    static void setTileSize(float width, float height, float scale);
    static std::vector<PropInstance>& getPropInstances();

private:
    static float tileWidth, tileHeight, tileScale;
    static sf::RenderWindow* window;
    static std::unordered_map<std::string, sf::Texture> textures;
    static sf::Clock lastPlaced;
    static std::vector<PropInstance> propInstances;
};

#endif // PROPSMANAGER_H