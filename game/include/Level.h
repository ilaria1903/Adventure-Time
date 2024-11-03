#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <vector>

class Level {
public:
    Level(int width, int height);
    Level(const Level& other);

    void addObstacle(int x, int y);
    void removeObstacle(int x, int y);
    bool isObstacle(int x, int y) const;

    friend std::ostream& operator<<(std::ostream& os, const Level& level);

private:
    int width, height;
    std::vector<std::vector<bool>> obstacles;
};

#endif // LEVEL_H