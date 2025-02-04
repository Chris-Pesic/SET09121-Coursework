#include "level_manager.hpp"
#include "misc.hpp"
#include <SFML/Graphics.hpp>

void LevelManager::update(sf::RenderWindow &window, float dt) {
    // Check collision
    for (int i = 0; i < this->entities.size(); i++) {
        // Check platforms
        for (int j = 0; j < this->platforms.size(); j++) {
            auto e = *this->entities.at(i);
            auto p = *this->platforms.at(j);

            if (e.getCollision().getGlobalBounds().intersects(p.getGlobalBounds())) {
                // do the collision
                this->entities.at(i)->collide("Platform");
            }
        }
        // Check other entities
        for (int j = 0; j < this->entities.size(); j++) {
            if (i == j)
                continue;

            auto e1 = *this->entities.at(i);
            auto e2 = *this->entities.at(j);

            if (isIntersecting(e1.getCollision(), e2.getCollision())) {
                // do the collision
            }
        }
    }


    // Platforms
    for (int i = 0; i < this->platforms.size(); i++) {
        window.draw(*this->platforms.at(i));
    }
    // Entities
    for (int i = 0; i < this->entities.size(); i++) {
        this->entities.at(i)->update(window, dt);
        this->entities.at(i)->render(window);
    }
}

void LevelManager::addEntity(Entity &entity) {
    this->entities.push_back(&entity);
}
void LevelManager::removeEntity(int index) {
    this->entities.erase(this->entities.begin() + index);
}

void LevelManager::addPlatform(sf::RectangleShape &platform) {
    this->platforms.push_back(&platform);
}
void LevelManager::removePlatform(int index) {
    this->platforms.erase(this->platforms.begin() + index);
}
