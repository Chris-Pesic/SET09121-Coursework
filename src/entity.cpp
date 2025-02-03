#include "entity.hpp"

void Entity::move() {
    collision.setPosition(position);
    sprite.setPosition(position);
}

sf::CircleShape Entity::getCollision() {
    return this->collision;
}
void Entity::setCollision(sf::CircleShape collision) {
    this->collision = collision;
}

sf::Vector2f Entity::getPosition() {
    return this->position;
}
void Entity::setPosition(sf::Vector2f position) {
    this->position = position;
}
