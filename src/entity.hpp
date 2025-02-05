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

class FauxPlayer : public Entity {
public:
    FauxPlayer(float sx, float sy) {
        speed = {300.f, 700.f};
        velocity = 0.f;

        tag = "Player";

        position = {sx, sy};
        move();

        spriteLocation = {1, 1, 110, 125};
        collision.setSize({55, 62.5f});

        if (!spritesheet.loadFromFile("./res/sprite/player.png")) {
            std::cerr << __FILE__ << ":" << __LINE__ << ": ERROR: Coudn't load player spritesheet!" << std::endl;
            exit(-1);
        }

        sprite.setTexture(spritesheet);
        sprite.setTextureRect(spriteLocation);
        sprite.setScale(0.5f, 0.5f);
    }

    std::string collide(std::string object) override {
        if (object == "Enemy") {
            std::cout << std::endl << "Player says \"i am kill\"" << std::endl;
            return "PlayerDeath";
        }

        return "";
    }

    void addPlatforms(std::vector<sf::RectangleShape*> platforms) {
        this->platforms = platforms;
    }

    void update(sf::RenderWindow &window, float dt) override {
        direction = 0;
        isGrounded = false;

        const float playerX = position.x;
        const float playerY = position.y;
        const float playerWidth = collision.getSize().x;
        const float playerHeight = collision.getSize().y;

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

        // Check platform collision
        for (int i = 0; i < platforms.size(); i++) {
            auto p = platforms.at(i);

            float platformLeft = p->getPosition().x;
            float platformRight = p->getPosition().x + p->getSize().x;
            float platformTop = p->getPosition().y;
            float platformBottom = p->getPosition().y + p->getSize().y;

            if ((playerX + playerWidth) > platformLeft && playerX < platformRight && // Top of platform
                (playerY + playerHeight) >= platformTop && (playerY + playerHeight) < platformBottom && !isGrounded) {
                velocity = 0.f;
                position.y = platformTop - playerHeight;
                isGrounded = true;
            } else if (((playerX + playerWidth) > platformLeft && playerX < platformLeft) && // Left of platform
                       ((playerY + playerHeight) > platformTop && playerY < platformBottom)) {
                direction = 0;
                position.x = (platformLeft - playerWidth);
            } else if ((playerX < platformRight && (playerX + playerWidth) > platformRight) && // Right of platform
                       ((playerY + playerHeight) > platformTop && playerY < platformBottom)) {
                direction = 0;
                position.x = platformRight;
            } else if ((playerX + playerWidth) > platformLeft && (playerX + playerWidth) < platformRight && // Bottom of platform
                       playerY <= (platformBottom + 1) && playerY > (platformTop + 1)) {
                velocity = 0.f;
                position.y = platformBottom + 1;
            }
        }

        // Gravity
        if (!isGrounded) {
            velocity += GRAVITY;
            spriteState = FALLING;
        }

        // Jumping
        if (isGrounded && sf::Keyboard::isKeyPressed(controls[0])) {
            velocity -= speed.y;
            spriteState = RISING;
            isGrounded = false;
        }

        // Move the player
        this->position += {this->direction * this->speed.x * dt, velocity * dt};
        move();
    }

    void render(sf::RenderWindow &window) override {
        // Animate the sprite
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
    std::vector<sf::RectangleShape*> platforms;
};

#endif /* ENTITY_HPP */
