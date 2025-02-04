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

    virtual void update(sf::RenderWindow &window, float dt) {}
    virtual void render(sf::RenderWindow &window) {}

    virtual std::string collide(std::string object) {} // `object' is the name of whatever has collided with the entity
                                                       // will return a string to tell whatever called it what to do
protected:
    sf::Texture spritesheet;
    sf::Sprite sprite;
    sf::IntRect spriteLocation;
    sf::Clock animationClock;
    sf::RectangleShape collision;
    sf::Vector2f lastValidPosition;
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
        collision.setSize({110/2, 125/2});

        if (!spritesheet.loadFromFile("./res/sprite/player.png")) {
            std::cerr << __FILE__ << ":" << __LINE__ << ": ERROR: Coudn't load player spritesheet!" << std::endl;
        } else {
            sprite.setTexture(spritesheet);
            sprite.setTextureRect(spriteLocation);
            sprite.setScale(0.5f, 0.5f);
        }
    }

    std::string collide(std::string object) override {
        if (object == "Platform") {
            // I deserve the death penalty for this
            if (position.y > this->lastValidPosition.y) {
                this->position.y = this->lastValidPosition.y;
                isGrounded = true;
            } else if (position.y < this->lastValidPosition.y) {
                this->position.y = this->lastValidPosition.y;
            } else if (position.x > this->lastValidPosition.x) {
                this->position.x = this->lastValidPosition.x;
            } else if (position.x < this->lastValidPosition.x) {
                this->position.x = this->lastValidPosition.x;
            }
            move();
        }

        return "";
    }

    void update(sf::RenderWindow &window, float dt) override {
        direction = 0;

        // Walking
        if (sf::Keyboard::isKeyPressed(controls[1])) {
            direction = -1;
            spriteState = WALKING_LEFT;
        } else if (sf::Keyboard::isKeyPressed(controls[2])) {
            direction = 1;
            spriteState = WALKING_RIGHT;
        } else {
            spriteState = STANDING;
        }

        // Gravity
        if (!isGrounded) {
            velocity += GRAVITY;
            spriteState = FALLING;
        } else {
            velocity = 0;
        }

        if (this->isGrounded && sf::Keyboard::isKeyPressed(controls[0])) {
            velocity -= 700.f;
            spriteState = RISING;
            isGrounded = false;
        }

        // Move the player
        this->lastValidPosition = this->position;
        this->position += {this->direction * this->speed.x * dt, velocity * dt};
        move();
    }

    void render(sf::RenderWindow &window) override {
        switch (spriteState) {
        case EggState::STANDING:
            spriteLocation.top = 1;
            // Animate grids 1-3
            if (animationClock.getElapsedTime().asSeconds() > 0.2f) {
                if (spriteLocation.left == 223)
                    spriteLocation.left = 1;
                else
                    {
                        spriteLocation.left += 111;
                    }

                sprite.setTextureRect(spriteLocation);
                animationClock.restart();
            }
            sprite.setScale(0.5f, 0.5f); // Ensure normal scale
            sprite.setOrigin(0, 0);      // Reset origin
            break;

        case EggState::WALKING_LEFT:
            // Animate grids 4-6 (left-facing)
            spriteLocation.top = 126;
            if (animationClock.getElapsedTime().asSeconds() > 0.2f) {
                if (spriteLocation.left == 223)
                    spriteLocation.left = 1;
                else
                    {
                        spriteLocation.left += 111;
                    }

                sprite.setTextureRect(spriteLocation);
                animationClock.restart();
            }
            sprite.setScale(-0.5f, 0.5f); // Flipped horizontally
            sprite.setOrigin(110, 0);     // Adjust origin for flipping
            break;
        case EggState::WALKING_RIGHT:
            spriteLocation.top = 126;
            // Animate grids 4-6 (right-facing)
            if (animationClock.getElapsedTime().asSeconds() > 0.2f) {
                if (spriteLocation.left == 223)
                    spriteLocation.left = 1;
                else
                    spriteLocation.left += 111;

                sprite.setTextureRect(spriteLocation);
                animationClock.restart();
            }
            sprite.setScale(0.5f, 0.5f); // Normal orientation
            sprite.setOrigin(0, 0);      // Reset origin
            break;

        case EggState::RISING:
            // Static frame for RISING
            sprite.setTextureRect(sf::IntRect(223, 2, 110, 125)); // Grid 3
            sprite.setScale(0.5f, 0.5f); // Ensure normal scale
            sprite.setOrigin(0, 0);      // Reset origin
            break;

        case EggState::FALLING:
            // Static frame for FALLING
            sprite.setTextureRect(sf::IntRect(111, 253, 110, 125)); // Grid 8
            sprite.setScale(0.5f, 0.5f);
            sprite.setOrigin(0, 0);
            break;

        case EggState::DYING:
            spriteLocation.top = 379;
            // Animate grids 4-6 (right-facing)
            if (animationClock.getElapsedTime().asSeconds() > 0.4f) {
                if (spriteLocation.left == 223)
                    break;
                else
                    spriteLocation.left += 111;

                sprite.setTextureRect(spriteLocation);
                animationClock.restart();
            }
            sprite.setScale(0.5f, 0.5f); // Normal orientation
            sprite.setOrigin(0, 0);      // Reset origin
            break;
        default:
            break;
        }

        // Render the sprite
        window.draw(this->sprite);
    }
private:
    bool isGrounded;
    EggState spriteState;
};

#endif /* ENTITY_HPP */
