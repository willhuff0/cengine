//
// Created by Will on 7/8/24.
//

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "visual_state.h"
#include "game/player.h"

typedef struct {
    LocalPlayer localPlayer;
} GameState;

void initGameState();
void freeGameState();

void tickGameState();

void visualizeGameState(VisualState* dest);

#endif //GAME_STATE_H
