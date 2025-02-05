#ifndef LEVEL_OBJECTS_HPP
#define LEVEL_OBJECTS_HPP

#include "entity.hpp"

class EnemyMoveLeft : public Entity {
public:
    EnemyMoveLeft(float x, float y, float w, float h);
};

class EnemyMoveRight : public Entity {
public:
    EnemyMoveRight(float x, float y, float w, float h);
};

#endif /* LEVEL_OBJECTS_HPP */
