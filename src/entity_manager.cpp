#include "entity_manager.hpp"
#include <memory>

void EntityManager::update(sf::RenderWindow &window, float dt) {
    for (int i = 0; i < this->entities.size(); i++) {
        this->entities.at(i)->update(window, dt);
        this->entities.at(i)->render(window);
    }
}

int EntityManager::addEntity(Entity &entity) {
    this->entities.push_back(&entity);
    return this->entities.size();
}

void EntityManager::removeEntity(int index) {
    this->entities.erase(this->entities.begin() + index);
}
