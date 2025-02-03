#include "entity.hpp"

void Entity::move() {
    collision.setPosition(position);
    sprite.setPosition(position);
}

sf::Texture Entity::getSpritesheet() {
    return this->spritesheet;
}
void Entity::setSpritesheet(sf::Texture spritesheet) {
    this->spritesheet = spritesheet;
}

sf::Sprite Entity::getSprite() {
    return this->sprite;
}
void Entity::setSprite(sf::Sprite sprite) {
    this->sprite = sprite;
}

sf::IntRect Entity::getSpriteLocation() {
    return this->spriteLocation;
}
void Entity::setSpriteLocation(sf::IntRect spriteLocation) {
    this->spriteLocation = spriteLocation;
}

sf::Clock Entity::getAnimationClock() {
    return this->animationClock;
}
void Entity::setAnimationClock(sf::Clock animationClock) {
    this->animationClock = animationClock;
}

sf::CircleShape Entity::getCollision() {
    return this->collision;
}
void Entity::setCollision(sf::CircleShape collision) {
    this->collision = collision;
}

sf::Vector2f Entity::getVelocity() {
    return this->velocity;
}
void Entity::setVelocity(sf::Vector2f velocity) {
    this->velocity = velocity;
}

sf::Vector2f Entity::getPosition() {
    return this->position;
}
void Entity::setPosition(sf::Vector2f position) {
    this->position = position;
}
