//
// Created by Will on 7/2/24.
//

#include "physics.h"

static dWorldID world;

void initPhysics() {
    dInitODE();
    world = NULL;
}
void freePhysics() {
    if (world != NULL) {
        dWorldDestroy(world);
    }
    dCloseODE();
}

static void copyOdeQuatToVersor(const dQuaternion from, versor dest) {
    dest[0] = from[1];
    dest[1] = from[2];
    dest[2] = from[3];
    dest[3] = from[0];
}

void physicsCreateWorld() {
    if (world != NULL) {
        dWorldDestroy(world);
    }
    world = dWorldCreate();
    dWorldSetGravity(world, 0.0f, PHYSICS_GRAVITY, 0.0f);
}

dBodyID physicsCreateSphere(vec3 pos, float radius, float mass) {
    dMass d_mass;
    dMassSetZero(&d_mass);
    dMassSetSphereTotal(&d_mass, mass, radius);

    dBodyID body = dBodyCreate(world);
    dBodySetMass(body, &d_mass);
    dBodySetPosition(body, pos[0], pos[1], pos[2]);

    return body;
}

void physicsRemoveBody(dBodyID body) {
    dBodyDestroy(body);
}