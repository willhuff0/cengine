//
// Created by Will on 7/2/24.
//

#ifndef PHYSICS_H
#define PHYSICS_H

#include "common.h"

#define PHYSICS_GRAVITY -9.8f

void initPhysics();
void freePhysics();

void physicsCreateWorld();
dBodyID physicsCreateSphere(vec3 pos, float radius, float mass);
void physicsRemoveBody(dBodyID body);

#endif //PHYSICS_H
