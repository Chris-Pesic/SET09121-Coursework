#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include "entity.hpp"
#include <SFML/Graphics.hpp>

class LevelManager {
public:
    void update(sf::RenderWindow &window, float dt); // Updates and renders entities and platforms

    void addEntity(Entity &entity);
    void removeEntity(int index);

    void addPlatform(sf::RectangleShape &platform);
    void removePlatform(int index);
    std::vector<sf::RectangleShape*> getPlatforms();
private:
    std::vector<Entity*> entities;
    std::vector<sf::RectangleShape*> platforms;
};

#endif /* LEVEL_MANAGER_HPP */
