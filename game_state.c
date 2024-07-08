//
// Created by Will on 7/8/24.
//

#include "game_state.h"

static GameState state;

void initGameState() {
    createLocalPlayer(&state.localPlayer, 0);
}
void freeGameState() {
    destroyLocalPlayer(&state.localPlayer);
}

void tickGameState() {
    tickLocalPlayer(&state.localPlayer);
}

void visualizeGameState(VisualState* dest) {
    visualizeLocalPlayer(&state.localPlayer, &dest->player);

    dest->timestamp = glfwGetTime();
}