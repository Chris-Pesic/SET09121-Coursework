#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "player.hpp"
#include "misc.hpp"

void Player::setState(EggState newState) {
    state = newState;
}

EggState Player::getState() const {
    return state;
}

void Player::updateState(sf::Sprite &eggsprite) {
    switch (getState()) {
    case EggState::STANDING:
        eggSourceSprite.top = 1;
        // Animate grids 1-3
        if (animateClock.getElapsedTime().asSeconds() > 0.2f) {
            if (eggSourceSprite.left == 223)
                eggSourceSprite.left = 1;
            else
            {
                eggSourceSprite.left += 111;
            }

            eggsprite.setTextureRect(eggSourceSprite);
            animateClock.restart();
        }
        eggsprite.setScale(0.5f, 0.5f); // Ensure normal scale
        eggsprite.setOrigin(0, 0);      // Reset origin
        break;

    case EggState::WALKING_LEFT:
        // Animate grids 4-6 (left-facing)
        eggSourceSprite.top = 126;
        if (animateClock.getElapsedTime().asSeconds() > 0.2f) {
            if (eggSourceSprite.left == 223)
                eggSourceSprite.left = 1;
            else
            {
                eggSourceSprite.left += 111;
            }

            eggsprite.setTextureRect(eggSourceSprite);
            animateClock.restart();
        }
        eggsprite.setScale(-0.5f, 0.5f); // Flipped horizontally
        eggsprite.setOrigin(110, 0);     // Adjust origin for flipping
        break;

    case EggState::WALKING_RIGHT:
        eggSourceSprite.top = 126;
        // Animate grids 4-6 (right-facing)
        if (animateClock.getElapsedTime().asSeconds() > 0.2f) {
            if (eggSourceSprite.left == 223)
                eggSourceSprite.left = 1;
            else
                eggSourceSprite.left += 111;

            eggsprite.setTextureRect(eggSourceSprite);
            animateClock.restart();
        }
        eggsprite.setScale(0.5f, 0.5f); // Normal orientation
        eggsprite.setOrigin(0, 0);      // Reset origin
        break;

    case EggState::RISING:
        // Static frame for RISING
        eggsprite.setTextureRect(sf::IntRect(223, 2, 110, 125)); // Grid 3
        eggsprite.setScale(0.5f, 0.5f); // Ensure normal scale
        eggsprite.setOrigin(0, 0);      // Reset origin
        break;

    case EggState::FALLING:
        // Static frame for FALLING
        eggsprite.setTextureRect(sf::IntRect(111, 253, 110, 125)); // Grid 8
        eggsprite.setScale(0.5f, 0.5f);
        eggsprite.setOrigin(0, 0);
        break;

    case EggState::DYING:
        eggSourceSprite.top = 379;
        // Animate grids 4-6 (right-facing)
        if (animateClock.getElapsedTime().asSeconds() > 0.4f) {
            if (eggSourceSprite.left == 223)
                break;
            else
                eggSourceSprite.left += 111;

            eggsprite.setTextureRect(eggSourceSprite);
            animateClock.restart();
        }
        eggsprite.setScale(0.5f, 0.5f); // Normal orientation
        eggsprite.setOrigin(0, 0);      // Reset origin
        break;
        eggsprite.setTextureRect(sf::IntRect(1, 379, 110, 125)); // Grid 10

    default:
        break;
    }
}

void Player::setSourceSprite(sf::IntRect eggSourceSprite) {
    this->eggSourceSprite = eggSourceSprite;
}
