#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Entity {
public:
    Entity() {}
    ~Entity() {}

    sf::Texture getSpritesheet();
    void setSpritesheet(sf::Texture spritesheet);
    sf::Sprite getSprite();
    void setSprite(sf::Sprite sprite);
    sf::IntRect getSpriteLocation();
    void setSpriteLocation(sf::IntRect spriteLocation);
    sf::Clock getAnimationClock();
    void setAnimationClock(sf::Clock animationClock);
    sf::CircleShape getCollision();
    void setCollision(sf::CircleShape collision);
    sf::Vector2f getVelocity();
    void setVelocity(sf::Vector2f velocity);
protected:
    void update();
    void render();
private:
    sf::Texture spritesheet;
    sf::Sprite sprite;
    sf::IntRect spriteLocation;
    sf::Clock animationClock;
    sf::CircleShape collision;
    sf::Vector2f velocity;
};

#endif /* ENTITY_HPP */
