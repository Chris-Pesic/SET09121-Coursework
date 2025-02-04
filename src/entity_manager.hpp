#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "entity.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class EntityManager {
public:
    void update(sf::RenderWindow &window, float dt); // Updates and renders entities

    int addEntity(Entity &entity); // Returns index of added entity
    void removeEntity(int index);
private:
    std::vector<Entity*> entities;
};

#endif /* ENTITY_MANAGER_HPP */
