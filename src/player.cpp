#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "player.hpp"
#include "debug.hpp"
#include "misc.hpp"

Player::Player(float sx, float sy) {
    speed = {300.f, 700.f};
    velocity = 0.f;
    direction = 0;

    tag = "Player";

    position = {sx, sy};

    spriteLocation = {1, 1, 110, 125};
    collision.setSize({45.f, 45.f});

    if (!spritesheet.loadFromFile("./res/sprite/player.png")) {
        std::cerr << __FILE__ << ":" << __LINE__ << ": ERROR: Coudn't load player spritesheet!" << std::endl;
        exit(-1);
    }

    sprite.setTexture(spritesheet);
    sprite.setTextureRect(spriteLocation);
    sprite.setScale(0.5f, 0.5f);

    move();
}

Player::~Player() {
    for (int i = platforms.size(); i > 0; i--) {
        delete platforms.at(i);
        platforms.pop_back();
    }
}

std::string Player::collide(std::string object) {
    if (object == "Enemy") {
        return "PlayerDeath";
    } else if (object == "Goal") {
        return "Goal";
    }

    return "";
}

void Player::addPlatforms(std::vector<sf::RectangleShape*> platforms) {
    this->platforms = platforms;
}

void Player::update(sf::RenderWindow &window, float dt) {
    direction = 0;
    isGrounded = false;

    const float playerX = position.x;
    const float playerY = position.y;
    const float playerWidth = collision.getSize().x;
    const float playerHeight = collision.getSize().y;

    // Walking
    if (sf::Keyboard::isKeyPressed(controls[1])) {
        direction = -1;
        spriteState = WALKING_LEFT;
    } else if (sf::Keyboard::isKeyPressed(controls[2])) {
        direction = 1;
        spriteState = WALKING_RIGHT;
    } else {
        spriteState = STANDING;
    }

    // Check platform collision
    for (int i = 0; i < platforms.size(); i++) {
        auto p = platforms.at(i);

        sf::RectangleShape top({p->getSize().x - 4, 5});
        top.setPosition({p->getPosition().x + 2, p->getPosition().y});
        sf::RectangleShape bottom({p->getSize().x - 4, 5});
        bottom.setPosition({p->getPosition().x + 2, (p->getPosition().y + p->getSize().y)});
        sf::RectangleShape left({5, p->getSize().y - 10});
        left.setPosition({p->getPosition().x, p->getPosition().y + 5});
        sf::RectangleShape right({5, p->getSize().y - 10});
        right.setPosition({(p->getPosition().x + p->getSize().x) - 5, p->getPosition().y + 5});

        if (collision.getGlobalBounds().intersects(left.getGlobalBounds())) {
            position.x = left.getPosition().x - playerWidth;
        } else if (collision.getGlobalBounds().intersects(right.getGlobalBounds())) {
            position.x = right.getPosition().x;
        } else if (collision.getGlobalBounds().intersects(bottom.getGlobalBounds())) {
            velocity = 0.f;
            position.y = (bottom.getPosition().y + bottom.getSize().y) + 1;
        } else if (collision.getGlobalBounds().intersects(top.getGlobalBounds())) {
            velocity = 0.f;
            position.y = top.getPosition().y - (playerHeight - 5);
            isGrounded = true;
        }

        #ifdef D_SHOW_WALL_COLLISION
        left.setFillColor(sf::Color::Magenta);
        right.setFillColor(sf::Color::Black);
        top.setFillColor(sf::Color::Blue);
        bottom.setFillColor(sf::Color::Red);

        window.draw(top);
        window.draw(bottom);
        window.draw(left);
        window.draw(right);
        #endif
    }

    // Gravity
    if (!isGrounded) {
        velocity += GRAVITY;
        spriteState = FALLING;
    }

    // Jumping
    if (isGrounded && sf::Keyboard::isKeyPressed(controls[0])) {
        velocity -= speed.y;
        spriteState = RISING;
        isGrounded = false;
    }

    // Clamp velocity
    clamp(velocity, -2000.f, 1000.f);

    // Move the player
    this->position += {this->direction * this->speed.x * dt, velocity * dt};
    collision.setPosition(position);
    sprite.setPosition({position.x-5, position.y-15});
}

void Player::render(sf::RenderWindow &window) {
    // Animate the sprite
    switch (spriteState) {
    case EggState::STANDING:
        spriteLocation.top = 1;
        // Animate grids 1-3
        if (animationClock.getElapsedTime().asSeconds() > 0.2f) {
            if (spriteLocation.left == 223)
                spriteLocation.left = 1;
            else
                {
                    spriteLocation.left += 111;
                }

            sprite.setTextureRect(spriteLocation);
            animationClock.restart();
        }
        sprite.setScale(0.5f, 0.5f); // Ensure normal scale
        sprite.setOrigin(0, 0);      // Reset origin
        break;

    case EggState::WALKING_LEFT:
        // Animate grids 4-6 (left-facing)
        spriteLocation.top = 126;
        if (animationClock.getElapsedTime().asSeconds() > 0.2f) {
            if (spriteLocation.left == 223)
                spriteLocation.left = 1;
            else
                {
                    spriteLocation.left += 111;
                }

            sprite.setTextureRect(spriteLocation);
            animationClock.restart();
        }
        sprite.setScale(-0.5f, 0.5f); // Flipped horizontally
        sprite.setOrigin(110, 0);     // Adjust origin for flipping
        break;
    case EggState::WALKING_RIGHT:
        spriteLocation.top = 126;
        // Animate grids 4-6 (right-facing)
        if (animationClock.getElapsedTime().asSeconds() > 0.2f) {
            if (spriteLocation.left == 223)
                spriteLocation.left = 1;
            else
                spriteLocation.left += 111;

            sprite.setTextureRect(spriteLocation);
            animationClock.restart();
        }
        sprite.setScale(0.5f, 0.5f); // Normal orientation
        sprite.setOrigin(0, 0);      // Reset origin
        break;

    case EggState::RISING:
        // Static frame for RISING
        sprite.setTextureRect(sf::IntRect(223, 2, 110, 125)); // Grid 3
        sprite.setScale(0.5f, 0.5f); // Ensure normal scale
        sprite.setOrigin(0, 0);      // Reset origin
        break;

    case EggState::FALLING:
        // Static frame for FALLING
        sprite.setTextureRect(sf::IntRect(111, 253, 110, 125)); // Grid 8
        sprite.setScale(0.5f, 0.5f);
        sprite.setOrigin(0, 0);
        break;

    case EggState::DYING:
        spriteLocation.top = 379;
        // Animate grids 4-6 (right-facing)
        if (animationClock.getElapsedTime().asSeconds() > 0.4f) {
            if (spriteLocation.left == 223)
                break;
            else
                spriteLocation.left += 111;

            sprite.setTextureRect(spriteLocation);
            animationClock.restart();
        }
        sprite.setScale(0.5f, 0.5f); // Normal orientation
        sprite.setOrigin(0, 0);      // Reset origin
        break;
    default:
        break;
    }

    // Render the sprite
    window.draw(this->sprite);
}
