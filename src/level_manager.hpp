#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include "entity.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <tinyxml2.h>

class LevelManager {
public:
    std::string update(sf::RenderWindow &window, float dt); // Updates and renders entities and platforms

    void addEntity(Entity* entity);
    void removeEntity(int index);
    void clearEntities();

    void addPlatform(sf::RectangleShape* platform);
    void removePlatform(int index);
    std::vector<sf::RectangleShape*> getPlatforms();
    void clearPlatforms();

    void resetLevel();
    void loadLevel(std::string path);
private:
    std::vector<Entity*> entities;
    std::vector<sf::RectangleShape*> platforms;
};

#endif /* LEVEL_MANAGER_HPP */
