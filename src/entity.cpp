#include "entity.hpp"

void Entity::move() {
    collision.setPosition(position);
    sprite.setPosition(position);
}

sf::RectangleShape Entity::getCollision() {
    return this->collision;
}
void Entity::setCollision(sf::RectangleShape collision) {
    this->collision = collision;
}

sf::Vector2f Entity::getPosition() {
    return this->position;
}
void Entity::setPosition(sf::Vector2f position) {
    this->position = position;
}

void Entity::setTag(std::string tag) {
    this->tag = tag;
}
std::string Entity::getTag() {
    return this->tag;
}
