#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(float width, float height);
    void startFollowing(const sf::Vector2f& newTarget);
    // void stopFollowing();
    void update(float deltaTime, sf::Vector2f target);
    void apply(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return view.getCenter(); }
    sf::View getView() const { return view; }

    friend std::ostream& operator<<(std::ostream& os, const Camera& camera);

private:
    sf::View view;
    sf::Vector2f target;
    bool isFollowing;
    float maxSpeed;
    float acceleration;
};

#endif // CAMERA_H