#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "misc.hpp"

class Player {
public:
    // Constructor to initialize the player state
    Player() : state(STANDING), frameCounter(0), animationSpeed(10), eggSourceSprite(1, 1, 110, 125) {}

    // Method to set the player's state
    void setState(EggState newState);

    // Method to get the current player's state
    EggState getState() const;

    void updateState(sf::Sprite &eggsprite);

    void setSourceSprite(sf::IntRect eggSourceSprite);
private:
    int frameCounter;    // Tracks which frame of the animation to show
    int animationSpeed;  // Controls animation speed (higher = slower animation)
    EggState state;      // Stores the current state of the player

    sf::Clock animateClock;

    sf::IntRect eggSourceSprite;
    sf::Texture spritesheet;
    sf::Sprite eggsprite;
};


#endif /* PLAYER_HPP */
