#include "../include/Camera.h"

Camera::Camera(int x, int y)
    : x(x), y(y) {}

Camera::Camera(const Camera& other)
    : x(other.x), y(other.y) {}

//void Camera::follow(int targetX, int targetY) {
//    this->x = targetX;
//   this->y = targetY;
// }

void Camera::setPosition(int newX, int newY) {
    this->x = newX;
    this->y = newY;
}

std::ostream& operator<<(std::ostream& os, const Camera& camera) {
    os << "Camera(Position: (" << camera.x << ", " << camera.y << "))";
    return os;
}