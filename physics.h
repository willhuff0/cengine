//
// Created by Will on 7/2/24.
//

#ifndef PHYSICS_H
#define PHYSICS_H

#include "common.h"
#include <pthread.h>
#include <ode/ode.h>

#include "node.h"

typedef enum {
    INIT_SCENE,
    ADD_NODE,
    REMOVE_NODE,
} PhysicsOp;

extern pthread_mutex_t physicsMutex;
extern vec3 ballPos;

void initPhysics();
void freePhysics();

void physicsInitScene();
void physicsAddNode(Node* node);
void physicsRemoveNode(dBodyID body);

#endif //PHYSICS_H
