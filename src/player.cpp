#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "misc.hpp"

void Player::setState(EggState newState) {
    state = newState;
}

EggState Player::getState() const {
    return state;
}
