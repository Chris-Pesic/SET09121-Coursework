#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "misc.hpp"

class Entity {
public:
    void move(); // Moves `collision' and `sprite' to `position'

    sf::RectangleShape getCollision();
    void setCollision(sf::RectangleShape collision);
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f position);
    std::string getTag();
    void setTag(std::string tag);

    virtual void update(sf::RenderWindow &window, float dt) {}
    virtual void render(sf::RenderWindow &window) {}

    virtual std::string collide(std::string object) {return "";} /* `object' is the tag of whatever has collided with the entity
                                                                    will return a string to tell whatever called it what to do */
protected:
    sf::Texture spritesheet;
    sf::Sprite sprite;
    sf::IntRect spriteLocation;
    sf::Clock animationClock;
    sf::RectangleShape collision;
    sf::Vector2f position;
    sf::Vector2f speed;
    int direction; // Horizontal
    float velocity;  // Vertical
    std::string tag;
};

#endif /* ENTITY_HPP */
