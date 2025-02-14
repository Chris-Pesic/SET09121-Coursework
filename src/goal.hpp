#ifndef GOAL_HPP
#define GOAL_HPP

#include "entity.hpp"

class Goal : public Entity {
public:
    Goal(float sx, float sy);

    void render(sf::RenderWindow &window) override;
};

#endif /* GOAL_HPP */
