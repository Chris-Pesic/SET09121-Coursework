#include "goal.hpp"

Goal::Goal(float sx, float sy) {
    speed = {0.f, 0.f};
    velocity = 0.f;

    position = {sx, sy};
    move();

    tag = "Goal";

    collision.setSize({150.f, 70.f});
    collision.setPosition({collision.getPosition().x - 25,
            collision.getPosition().y - 30});

    if (!spritesheet.loadFromFile("./res/sprite/goalNew.png")) {
        std::cerr << __FILE__ << ":" << __LINE__ << ": ERROR: Coudn't load enemy spritesheet!" << std::endl;
        exit(-1);
    }

    sprite.setTexture(spritesheet);
}

void Goal::render(sf::RenderWindow &window) {
    window.draw(sprite);
}
