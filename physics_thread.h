//
// Created by Will on 7/7/24.
//

#ifndef PHYSICS_THREAD_H
#define PHYSICS_THREAD_H

#include "physics.h"

typedef struct {
    pthread_mutex_t mutex;
    PhysicsQueueEntry* ops;
} PhysicsQueue;

extern PhysicsQueue physicsQueue;

void* physicsThreadEntry(void* _);
void physicsThreadActivateShutdownFlag();

#endif //PHYSICS_THREAD_H
