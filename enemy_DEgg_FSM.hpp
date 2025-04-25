#include "player.hpp"
#ifndef ENEMY_DEGG_FINITE_STATE_MACHINE_HPP
#define ENEMY_DEGG_FINITE_STATE_MACHINE_HPP


enum EnemyDEggState {
    Idle,
    Patrol,
    Target
};

class EnemyDEggStateMachine {
public:
    EnemyDEggStateMachine();
    void Update();
    void HandleIdle();
    void HandlePatrol();
    void HandleTarget();
    void TransitionState();
    EnemyDEggState GetCurrentState() const;

private:
    EnemyDEggState currentState;
    Player player;
};

#endif /* ENEMY_DEGG_FINITE_STATE_MACHINE_HPP */
