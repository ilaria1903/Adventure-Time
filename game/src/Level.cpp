#include "../include/Level.h"

Level::Level(int width, int height)
    : width(width), height(height), obstacles(width, std::vector<bool>(height, false)) {}

Level::Level(const Level& other)
    : width(other.width), height(other.height), obstacles(other.obstacles) {}

void Level::addObstacle(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        obstacles[x][y] = true;
    }
}

void Level::removeObstacle(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        obstacles[x][y] = false;
    }
}

bool Level::isObstacle(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return obstacles[x][y];
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Level& level) {
    os << "Level(" << level.width << "x" << level.height << ")";
    return os;
}