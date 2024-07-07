//
// Created by Will on 7/2/24.
//

#ifndef PHYSICS_H
#define PHYSICS_H

#include "common.h"
#include <pthread.h>
#include <ode/ode.h>

#include "node.h"
#include "components/trans_comp.h"

typedef enum {
    PHY_INIT_SCENE,
    PHY_ADD_BODY,
    PHY_REMOVE_BODY,
} PhysicsOp;

typedef union {
    PhysicsOp op;
    void* obj;
} PhysicsQueueEntry;

void initPhysics();
void freePhysics();

void physicsQueueStart();
void physicsQueueSubmit();

void physicsInitScene();
void physicsAddBody(PhysicsTransComp* comp);
void physicsRemoveBody(dBodyID body);

#endif //PHYSICS_H
