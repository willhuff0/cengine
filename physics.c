//
// Created by Will on 7/2/24.
//

#include "physics.h"

#include <ode/ode.h>

#include "physics_thread.h"
#include "components/trans_comp.h"

static PhysicsQueueEntry* opStack;

static pthread_t thread;

static void initThreadQueue() {
    pthread_mutex_init(&physicsQueue.mutex, NULL);
    physicsQueue.ops = NULL;
}

static void freeThreadQueue() {
    pthread_mutex_destroy(&physicsQueue.mutex);
    arrfree(physicsQueue.ops);
}

void initPhysics() {
    opStack = NULL;
    initThreadQueue();
    pthread_create(&thread, NULL, physicsThreadEntry, NULL);
}

void freePhysics() {
    physicsThreadActivateShutdownFlag();
    pthread_join(thread, NULL);
    if (opStack != NULL) arrfree(opStack);
    freeThreadQueue();
}

void physicsQueueStart() {
    if (opStack != NULL) {
        arrfree(opStack);
        opStack = NULL;
    }
}
void physicsQueueSubmit() {
    int count = arrlen(opStack);
    if (count == 0) return;

    pthread_mutex_lock(&physicsQueue.mutex);
    PhysicsQueueEntry* threadOpStack = arraddnptr(physicsQueue.ops, count);
    memcpy(threadOpStack, opStack, count * sizeof(PhysicsQueueEntry));
    pthread_mutex_unlock(&physicsQueue.mutex);
}

static void pushOp(PhysicsOp op) {
    arrput(opStack, (PhysicsQueueEntry){.op = op});
}
static void pushOpAndObj(PhysicsOp op, void* obj) {
    arrput(opStack, (PhysicsQueueEntry){.op = op});
    arrput(opStack, (PhysicsQueueEntry){.obj = obj});
}

void physicsInitScene() {
    pushOp(PHY_INIT_SCENE);
}

void physicsAddBody(PhysicsTransComp* comp) {
    pushOpAndObj(PHY_ADD_BODY, comp);
}

void physicsRemoveBody(dBodyID body) {
    pushOpAndObj(PHY_REMOVE_BODY, body);
}