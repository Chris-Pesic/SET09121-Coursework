#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "misc.hpp"

class Player {
public:
    // Constructor to initialize the player state
    Player() : state(STANDING), frameCounter(0), animationSpeed(10) {}

    // Method to set the player's state
    void setState(EggState newState);

    // Method to get the current player's state
    EggState getState() const;
private:
    int frameCounter;    // Tracks which frame of the animation to show
    int animationSpeed;  // Controls animation speed (higher = slower animation)
    EggState state;      // Stores the current state of the player

    sf::Texture spritesheet;
    sf::Sprite eggsprite;
};


#endif /* PLAYER_HPP */
