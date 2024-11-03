#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

class Camera {
public:
    Camera(int x, int y);
    Camera(const Camera& other);

    void follow(int targetX, int targetY);
    void setPosition(int x, int y);

    friend std::ostream& operator<<(std::ostream& os, const Camera& camera);

private:
    int x, y;
};

#endif // CAMERA_H