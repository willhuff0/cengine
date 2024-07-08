//
// Created by Will on 7/7/24.
//

#ifndef GAME_H
#define GAME_H

//#define DEBUG_TIMESTEP
#define MAX_TICKS_PER_TIMESTEP 4
#define GAME_TIMESTEP (1.0f / 60.0f)

typedef struct {
    float deltaTime;
} GameTickArgs;

extern GameTickArgs tickArgs;

void* gameThreadEntry(void* _);
void gameThreadActivateShutdownFlag();

#endif //GAME_H
