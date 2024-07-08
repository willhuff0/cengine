//
// Created by Will on 7/7/24.
//

#include "physics_thread.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <ode/ode.h>

#include "physics.h"
#include "components/trans_comp.h"

//#define DEBUG_PHYSICS

#define MAX_TICKS_PER_TIMESTEP 4
#define PHYSICS_TIMESTEP (1.0f / 30.0f)

PhysicsQueue physicsQueue;

static bool shutdown;
static dWorldID world;

static PhysicsTransComp** bodies;

static void physicsInit() {
    dInitODE();
    shutdown = false;
    world = NULL;
    bodies = NULL;
}

static void physicsFree() {
    if (world != NULL) {
        dWorldDestroy(world);
    }
    if (bodies != NULL) {
        arrfree(bodies);
    }
    dCloseODE();
}

static void phyInitScene() {
    if (world != NULL) {
        dWorldDestroy(world);
    }
    if (bodies != NULL) {
        arrfree(bodies);
        bodies = NULL;
    }
    world = dWorldCreate();
    dWorldSetGravity(world, 0.0f, -9.8f, 0.0f);
}
static void phyAddBody(PhysicsTransComp* obj) {
    pthread_mutex_lock(&obj->mutex);

    dMass mass;
    dMassSetZero(&mass);
    dMassSetSphereTotal(&mass, 1.0f, 0.5f);

    obj->body = dBodyCreate(world);
    dBodySetMass(obj->body, &mass);
    dBodySetPosition(obj->body, obj->trans.pos[0], obj->trans.pos[1], obj->trans.pos[2]);

    pthread_mutex_unlock(&obj->mutex);

    arrput(bodies, obj);
}
static void phyRemoveBody(dBodyID obj) {
    dBodyDestroy(obj);

    for (int i = arrlen(bodies) - 1; i >= 0; --i) {
        if (bodies[i]->body == obj) {
            arrdel(bodies, i);
            break;
        }
    }
}

static void physicsTick() {
    int i = 0;
    for(;;) {
        pthread_mutex_lock(&physicsQueue.mutex);
        int len = arrlen(physicsQueue.ops);
        if (i >= len) {
            pthread_mutex_unlock(&physicsQueue.mutex);
            break;
        }

        switch(physicsQueue.ops[0].op) {
            case PHY_INIT_SCENE: {
                arrdel(physicsQueue.ops, 0);
                pthread_mutex_unlock(&physicsQueue.mutex);
                phyInitScene();
                break;
            }
            case PHY_ADD_BODY: {
                PhysicsTransComp* obj = physicsQueue.ops[1].obj;
                arrdeln(physicsQueue.ops, 0, 2);
                pthread_mutex_unlock(&physicsQueue.mutex);
                phyAddBody(obj);
                break;
            }
            case PHY_REMOVE_BODY: {
                dBodyID obj = physicsQueue.ops[1].obj;
                arrdeln(physicsQueue.ops, 0, 2);
                pthread_mutex_unlock(&physicsQueue.mutex);
                phyRemoveBody(obj);
                break;
            }
        }

        i++;
    }

    if (world != NULL) {
        dWorldQuickStep(world, PHYSICS_TIMESTEP);

        for (int k = 0; k < arrlen(bodies); ++k) {
            PhysicsTransComp* comp = bodies[k];
            pthread_mutex_lock(&comp->mutex);
            glm_vec3_copy((float*)dBodyGetPosition(comp->body), comp->trans.pos);
            copyOdeQuatToVersor(dBodyGetQuaternion(comp->body), comp->trans.rot);
            pthread_mutex_unlock(&comp->mutex);
        }
    }
}

static void sleepForSeconds(double seconds) {
    struct timespec req;
    req.tv_sec = (time_t)seconds;
    req.tv_nsec = (long)((seconds - req.tv_sec) * 1e9);

    nanosleep(&req, NULL);
}

static double startTime;
static double getTime() {
    return glfwGetTime() - startTime;
}

void* physicsThreadEntry(void* _) {
    physicsInit();

    startTime = glfwGetTime();
    unsigned int tickIndex = 0;
    for(;;) {
        int i = 0;
        while (PHYSICS_TIMESTEP * tickIndex < getTime()) {
            physicsTick();
            tickIndex++;
            i++;
            if (i >= 4) {
                int skip = ceil((getTime() - PHYSICS_TIMESTEP * tickIndex) / PHYSICS_TIMESTEP);
                tickIndex += skip;
                printf("[PHYSICS] Running behind!, skipping %d tick(s).\n", skip);
                break;
            }

            if (shutdown) {
                physicsFree();
                return NULL;
            }
        }

        double wait = PHYSICS_TIMESTEP * tickIndex - getTime();
#ifdef DEBUG_PHYSICS
        printf("TICK %d: tt=%f, ct=%f, executed %d time(s), waiting %f seconds\n", tickIndex, PHYSICS_TIMESTEP * tickIndex, getTime(), i, wait);
#endif
        sleepForSeconds(wait);

        if (shutdown) {
            physicsFree();
            return NULL;
        }
    }
}

void physicsThreadActivateShutdownFlag() {
    shutdown = true;
}