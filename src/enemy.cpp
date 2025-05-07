#include "enemy.hpp"

Enemy::Enemy(float sx, float sy, float xspeed, int sdirection) {
    speed = {xspeed, 0.f};
    velocity = 0.f;
    direction = sdirection;

    position = {sx, sy};

    tag = "Enemy";

    spriteState2 = SEEKING;

    //spriteLocation = {223, 1, 110, 125};
    spriteLocation = { 1, 1, 290, 334 };
    collision.setSize({35.f, 45.f});

    if (!spritesheet.loadFromFile("./res/sprite/enemyNew.png")) {
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
    switch (spriteState2) {
    case DeggState::SEEKING:
        // Animate grids 1-8 on 2nd row
        spriteLocation.top = 334; //second row
        if (animationClockEnemy.getElapsedTime().asSeconds() > 0.03f) {
            if (spriteLocation.left == (1 + (290 * 7)))
                spriteLocation.left = 1;
            else
                spriteLocation.left += 290;
            
            sprite.setTextureRect(spriteLocation);
            animationClockEnemy.restart();
        }
        sprite.setScale(-0.2f, 0.2f); // Flipped horizontally
        sprite.setOrigin(290, 0);     // Adjust origin for flipping
        break;
    case DeggState::CHASING:
        // Animate grids 1-12 on 1st row
        spriteLocation.top = 1; //first row
        if (animationClockEnemy.getElapsedTime().asSeconds() > 0.03f) {
            if (spriteLocation.left == (1 + 290 * 11))
                spriteLocation.left = 1;
            else
                spriteLocation.left += 290;

            sprite.setTextureRect(spriteLocation);
            animationClockEnemy.restart();
        }
        sprite.setScale(0.2f, 0.2f); // Normal orientation
        sprite.setOrigin(0, 0);      // Reset origin
        break;

    case DeggState::ALERT:
        // Animate grids 9-11 on 2nd row
        spriteLocation.top = 334; //second row
        spriteLocation.left = 290 * 6;
        sprite.setTextureRect(spriteLocation);

        sprite.setScale(0.2f, 0.2f); // Ensure normal scale
        sprite.setOrigin(0, 0);      // Reset origin
        break;
    default:
        break;
    }

    // Render the sprite
    window.draw(this->sprite);
}
