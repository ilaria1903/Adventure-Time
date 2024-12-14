#include "../include/PropsManager.h"
#include "../include/Exception.h"
#include <iostream>

/*
std::unordered_map<std::string, sf::Texture> PropsManager::textures;
std::vector<PropInstance> PropsManager::propInstances;
sf::Clock PropsManager::lastPlaced;

sf::RenderWindow* PropsManager::window = nullptr;

float PropsManager::tileWidth = 0;
float PropsManager::tileHeight = 0;
float PropsManager::tileScale = 0;
*/

PropsManager::PropsManager() : tileWidth(0), tileHeight(0), tileScale(0), window(nullptr) {}

void PropsManager::loadProp(const std::string& name, const std::string& filePath) {
    sf::Texture texture;
    if (!texture.loadFromFile(filePath)) {
        // std::cerr << "Error: Failed to load texture: " << filePath << std::endl;
        throw FileLoadException("Failed to load texture in PropsManager::loadProp");
    }
    textures[name] = texture;
}

void PropsManager::addPropInstance(const std::string& name, float scale, int x, int y) {
    if (textures.find(name) == textures.end()) {
        // std::cerr << "Error: Texture not found for prop: " << name << std::endl;
        throw InvalidOperationException("Texture not found for prop in PropsManager::addPropInstance");
    }

    if (!window) {
        // std::cerr << "Error: Window not set in PropsManager::addPropInstance" << std::endl;
        throw InvalidOperationException("Window not set in PropsManager::addPropInstance");
    }

    if (tileWidth == 0 || tileHeight == 0 || tileScale == 0) {
        // std::cerr << "Error: Tile size not set in PropsManager::addPropInstance" << std::endl;
        throw InvalidOperationException("Tile size not set in PropsManager::addPropInstance");
    }

    if (x == 0 && y == 0) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        x = mousePos.x;
        y = mousePos.y;
    }

    if (lastPlaced.getElapsedTime().asSeconds() < 0.2f) {
        return;
    }

    // This is to snap the prop to the grid
    x = x / (tileWidth * tileScale);
    y = y / (tileHeight * tileScale);

    x *= tileWidth * tileScale;
    y *= tileHeight * tileScale;

    PropInstance instance;
    instance.sprite.setTexture(textures[name]);
    instance.sprite.setPosition(x, y);
    instance.x = x;
    instance.y = y;
    instance.scale = scale;
    instance.name = name;
    instance.sprite.setScale(scale, scale);
    propInstances.push_back(instance);

    lastPlaced.restart();
}

void PropsManager::removePropInstance(float _x, float _y) {
    if (!window) {
        // std::cerr << "Error: Window not set in PropsManager::removePropInstance" << std::endl;
        throw InvalidOperationException("Window not set in PropsManager::removePropInstance");
    }

    if (tileWidth == 0 || tileHeight == 0 || tileScale == 0) {
        // std::cerr << "Error: Tile size not set in PropsManager::removePropInstance" << std::endl;
        throw InvalidOperationException("Tile size not set in PropsManager::removePropInstance");
    }

    int x = _x;
    int y = _y;

    // Snap to grid
    x = (x / (tileWidth * tileScale)) * (tileWidth * tileScale);
    y = (y / (tileHeight * tileScale)) * (tileHeight * tileScale);

    // Check for intersection with prop instance
    for (auto it = propInstances.begin(); it != propInstances.end(); ++it) {
        if (x >= it->x && x <= it->x + tileWidth * tileScale &&
            y >= it->y && y <= it->y + tileHeight * tileScale) {
            propInstances.erase(it);
            return;
        }
    }
}

std::vector<PropInstance>& PropsManager::getPropInstances() {
    return propInstances;
}

void PropsManager::renderProps(sf::RenderWindow& _window) {
    for (const auto& instance : propInstances) {
        _window.draw(instance.sprite);
    }
}

void PropsManager::setWindow(sf::RenderWindow& _window) {
    PropsManager::window = &_window;
}

void PropsManager::setTileSize(float width, float height, float scale) {
    tileWidth = width;
    tileHeight = height;
    tileScale = scale;
}