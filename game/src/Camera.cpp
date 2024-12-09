#include "../include/Camera.h"

Camera::Camera(float width, float height) : view(sf::FloatRect(0, 0, width, height)), target(0, 0), isFollowing(false), maxSpeed(1000.0f), acceleration(800.0f) {
    view.setSize(width, height);
    view.setCenter(width, height / 2);
}

void Camera::update(float deltaTime, sf::Vector2f _target) {
    target = _target;
    if (isFollowing) {
        sf::Vector2f direction = target - view.getCenter();
        direction.y = 0; // Keep Y steady
        float distance = std::abs(direction.x); // Calculate distance only in X
        if (distance > 1.0f) { // If the distance is significant
            direction.x /= distance; // Normalize direction
            float speed = std::min(maxSpeed, acceleration * deltaTime * distance); // Speed proportional to distance
            view.move(direction.x * speed * deltaTime, 0); // Move smoothly only in X
        }
    }
}

void Camera::startFollowing(const sf::Vector2f& newTarget) {
    target = newTarget;
    isFollowing = true;
}

//void Camera::stopFollowing() {
 //   isFollowing = false;
//}

void Camera::apply(sf::RenderWindow& window) {
    window.setView(view);
}

std::ostream& operator<<(std::ostream& os, const Camera& camera) {
    os << "Camera view: " << camera.view.getCenter().x << ", " << camera.view.getCenter().y << std::endl;
    return os;
}