#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

class Entity {
public:
    void move(); // Moves `collision' and `sprite' to `position'

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
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f position);

    virtual void update() {}
    virtual void render(sf::RenderWindow &window) {}
protected:
    sf::Texture spritesheet;
    sf::Sprite sprite;
    sf::IntRect spriteLocation;
    sf::Clock animationClock;
    sf::CircleShape collision;
    sf::Vector2f velocity;
    sf::Vector2f position;
};

class FauxPlayer : public Entity {
public:
    FauxPlayer() {
        spriteLocation = {1, 1, 110, 125};

        if (!spritesheet.loadFromFile("./res/sprite/player.png")) {
            std::cerr << __FILE__ << ":" << __LINE__ << ": ERROR: Coudn't load player spritesheet!" << std::endl;
        }
        else {
            sprite.setTexture(spritesheet);
            sprite.setTextureRect(spriteLocation);
            sprite.setScale(0.5f, 0.5f);
        }
    }

    void update() override {
        this->position += this->velocity;
        move();
    }
    void render(sf::RenderWindow &window) override {
        window.draw(this->sprite);
    }
};

#endif /* ENTITY_HPP */
