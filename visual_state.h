//
// Created by Will on 7/7/24.
//

#ifndef VISUAL_STATE_H
#define VISUAL_STATE_H

#include "game/player.h"

struct VCamera {
    vec3 position;
    vec3 forward;
};

typedef struct {
    double timestamp;

    VCamera camera;
    VPlayer localPlayer;
} VisualState;

void initVisualStates();
void freeVisualStates();

void lockVisualStates();
void unlockVisualStates();

void swapVisualStates();

const VisualState* getPreviousVisualState();
const VisualState* getCurrentVisualState();
VisualState* getNextVisualState();

#endif //VISUAL_STATE_H
