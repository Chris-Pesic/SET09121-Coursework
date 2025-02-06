#include "level_manager.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

std::string LevelManager::update(sf::RenderWindow &window, float dt) {
    // Check collision
    for (int i = 0; i < this->entities.size(); i++) {
        for (int j = 0; j < this->entities.size(); j++) {
            if (i == j)
                continue;

            auto e1 = *this->entities.at(i);
            auto e2 = *this->entities.at(j);

            if (e1.getCollision().getGlobalBounds().intersects(e2.getCollision().getGlobalBounds())) {
                // do the collision
                std::string e1out = this->entities.at(i)->collide(e2.getTag());
                std::string e2out = this->entities.at(j)->collide(e1.getTag());

                if (e1out == "PlayerDeath" ||
                    e2out == "PlayerDeath") {
                    return "PlayerDeath";
                } else if (e1out == "Goal" ||
                           e2out == "Goal") {
                    return "Goal";
                }
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

    return "";
}

void LevelManager::addEntity(Entity* entity) {
    this->entities.push_back(entity);
}
void LevelManager::removeEntity(int index) {
    this->entities.erase(this->entities.begin() + index);
}
void LevelManager::clearEntities() {
    for (auto e : this->entities) {
        delete e;
    }
    this->entities.clear();
}

void LevelManager::addPlatform(sf::RectangleShape* platform) {
    this->platforms.push_back(platform);
}
void LevelManager::removePlatform(int index) {
    this->platforms.erase(this->platforms.begin() + index);
}
std::vector<sf::RectangleShape*> LevelManager::getPlatforms() {
    return this->platforms;
}
void LevelManager::clearPlatforms() {
    for (auto p : this->platforms) {
        delete p;
    }
    this->platforms.clear();
}
