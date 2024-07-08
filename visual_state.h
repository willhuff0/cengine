//
// Created by Will on 7/7/24.
//

#ifndef VISUAL_STATE_H
#define VISUAL_STATE_H

#include "game/player.h"

typedef struct {
    double timestamp;

    VPlayer player;
} VisualState;

void initVisualStates();
void freeVisualStates();

void swapVisualStates();

const VisualState* getPreviousVisualState();
const VisualState* getCurrentVisualState();
VisualState* getNextVisualState();

#endif //VISUAL_STATE_H
