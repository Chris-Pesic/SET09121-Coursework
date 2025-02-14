#include "level_objects.hpp"

EnemyMoveLeft::EnemyMoveLeft(float x, float y, float w, float h) {
    tag = "EnemyGoLeft";

    position = {x, y};
    collision.setSize({w, h});
    move();
}

EnemyMoveRight::EnemyMoveRight(float x, float y, float w, float h) {
    tag = "EnemyGoRight";

    position = {x, y};
    collision.setSize({w, h});
    move();
}
