#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "misc.hpp"

class Entity {
public:
    void move(); // Moves `collision' and `sprite' to `position'

    sf::CircleShape getCollision();
    void setCollision(sf::CircleShape collision);
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f position);

    virtual void update(sf::RenderWindow &window, float dt) {}
    virtual void render(sf::RenderWindow &window) {}
protected:
    sf::Texture spritesheet;
    sf::Sprite sprite;
    sf::IntRect spriteLocation;
    sf::Clock animationClock;
    sf::CircleShape collision;
    sf::Vector2f position;
    sf::Vector2f speed;
    int direction; // Horizontal
    float velocity;  // Vertical
};

class FauxPlayer : public Entity {
public:
    FauxPlayer() : isGrounded(false) {
        speed = {400, 1200};

        spriteLocation = {1, 1, 110, 125};

        if (!spritesheet.loadFromFile("./res/sprite/player.png")) {
            std::cerr << __FILE__ << ":" << __LINE__ << ": ERROR: Coudn't load player spritesheet!" << std::endl;
        } else {
            sprite.setTexture(spritesheet);
            sprite.setTextureRect(spriteLocation);
            sprite.setScale(0.5f, 0.5f);
        }
    }

    void update(sf::RenderWindow &window, float dt) override {
        direction = 0;

        if (sf::Keyboard::isKeyPressed(controls[1])) {
            direction = -1;
        } else if (sf::Keyboard::isKeyPressed(controls[2])) {
            direction = 1;
        }

        if (isGrounded == false) {
            velocity += GRAVITY;
        } else {
            velocity = 0;
        }

        if (position.y >= 500) {
            isGrounded = true;
        }

        this->position += {this->direction * this->speed.x * dt, velocity * dt};
        move();
    }
    void render(sf::RenderWindow &window) override {
        window.draw(this->sprite);
    }
private:
    bool isGrounded;
};

#endif /* ENTITY_HPP */
