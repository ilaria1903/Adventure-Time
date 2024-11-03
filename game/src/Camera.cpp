#include "../include/Camera.h"

Camera::Camera(int x, int y)
    : x(x), y(y) {}

Camera::Camera(const Camera& other)
    : x(other.x), y(other.y) {}

void Camera::follow(int targetX, int targetY) {
    x = targetX;
    y = targetY;
}

void Camera::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

std::ostream& operator<<(std::ostream& os, const Camera& camera) {
    os << "Camera(Position: (" << camera.x << ", " << camera.y << "))";
    return os;
}