#ifndef MISC_HPP
#define MISC_HPP

#include <SFML/Graphics.hpp>

#define GRAVITY 24.f

#define isIntersecting(r1, r2) (r1.getGlobalBounds().intersects(r2.getGlobalBounds()))

const sf::Keyboard::Key controls[3] = {
    sf::Keyboard::W,   // Player1 Up
    sf::Keyboard::A,   // Player1 Left
    sf::Keyboard::D,  // Player1 Right
};

// Define an enum for player states
enum EggState {
    STANDING = 0,
    WALKING_LEFT,
    WALKING_RIGHT,
    RISING,
    FALLING,
    DYING
};

#endif /* MISC_HPP */
