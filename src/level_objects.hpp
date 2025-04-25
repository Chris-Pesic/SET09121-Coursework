#ifndef LEVEL_OBJECTS_HPP
#define LEVEL_OBJECTS_HPP

#include "entity.hpp"

/**
 * @brief Makes the enemy move left when hit.
 */
class EnemyMoveLeft : public Entity {
public:
    /**
     * @brief Set's the position and size of the object. Run's at object creation.
     * @param x x position of the object.
     * @param y y position of the object.
     * @param w width of the object.
     * @param h height of the object.
     */
    EnemyMoveLeft(float x, float y, float w, float h);
};

/**
 * @brief Makes the enemy move right when hit.
 */
class EnemyMoveRight : public Entity {
public:
    /**
     * @brief Set's the position and size of the object. Run's at object creation.
     * @param x x position of the object.
     * @param y y position of the object.
     * @param w width of the object.
     * @param h height of the object.
     */
    EnemyMoveRight(float x, float y, float w, float h);
};

#endif /* LEVEL_OBJECTS_HPP */
