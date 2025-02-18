#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "entity.hpp"

class Enemy : public Entity {
public:
    Enemy(float sx, float sy, float xspeed, int sdirection);

    std::string collide(std::string object) override;
    void update(sf::RenderWindow &window, float dt) override;
    void render(sf::RenderWindow &window) override;
};

#endif /* ENEMY_HPP */
