#include "enemy_DEgg_FSM.hpp"
#include "enemy.hpp"
#include "entity.hpp"
#include "player.hpp"
#include "level_objects.hpp"

EnemyDEggStateMachine::EnemyDEggStateMachine() : currentState(Idle) {}

void EnemyDEggStateMachine::Update() {
    switch (currentState) {
    case Idle:
        HandleIdle();
        TransitionState();
        break;

    case Patrol:
        HandlePatrol();
        TransitionState();
        break;

    case Target:
        HandleTarget();
        TransitionState();
        break;
    }
}

void EnemyDEggStateMachine::TransitionState() {
    //To Target
    /*
    if (player.getLocalBounds().intersects(DetectionBox.getLocalBounds())) {
        currentState = Target;
    }

    //To Patrol
    if (!(Player->getLocalBounds()->intersects(DetectionBox->getLocalBounds())) && currentstate == Target) {
        currentState = Patrol;
    }
    */
    //To Idle not available after targeting to increase difficulty
}

void EnemyDEggStateMachine::HandleIdle() {
    // Idle Code
    /*
    Spin in place at the centre of the platform
    *//*
}

void EnemyDEggStateMachine::HandlePatrol() {
    // Patrol Code
    /*
    Using the spin animation, move to the outer bounds of the platform, colliding with the emr and eml
    *//*
}

void EnemyDEggStateMachine::HandleTarget() {
    // Target Code
    /*
    Use the run animation to move to the player's position
    *//*
}

EnemyDEggState EnemyDEggStateMachine::GetCurrentState() const {
    return this->currentState;
}*/