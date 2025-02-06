#include "enemy.hpp"

Enemy::Enemy(float sx, float sy, float xspeed, int sdirection) {
    speed = {xspeed, 0.f};
    velocity = 0.f;
    direction = sdirection;

    position = {sx, sy};

    tag = "Enemy";

    spriteLocation = {223, 1, 110, 125};
    collision.setSize({35.f, 45.f});

    if (!spritesheet.loadFromFile("./res/sprite/enemy.png")) {
        std::cerr << __FILE__ << ":" << __LINE__ << ": ERROR: Coudn't load enemy spritesheet!" << std::endl;
        exit(-1);
    }

    sprite.setTexture(spritesheet);
    sprite.setTextureRect(spriteLocation);
    sprite.setScale(0.5f, 0.5f);
}

std::string Enemy::collide(std::string object) {
    if (object == "EnemyGoLeft") {
        direction = -1;
    } else if (object == "EnemyGoRight") {
        direction = 1;
    }

    return "";
}

void Enemy::update(sf::RenderWindow &window, float dt) {
    // Move the player
    this->position += {this->direction * this->speed.x * dt, velocity * dt};
    move();
    this->collision.setPosition({collision.getPosition().x + 10, collision.getPosition().y + 15});
}

void Enemy::render(sf::RenderWindow &window) {
    // Render the sprite
    window.draw(this->sprite);
}
