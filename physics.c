//
// Created by Will on 7/2/24.
//

#include "physics.h"

#include <ode/ode.h>

//#define DEBUG_PHYSICS

#define MAX_TICKS_PER_TIMESTEP 4
#define PHYSICS_TIMESTEP (1.0f / 30.0f)

pthread_mutex_t physicsMutex;
vec3 ballPos = {0.0f, 50.0f, 0.0f};

static pthread_t thread;
static bool shutdown;

static dWorldID world;
static dBodyID ball;

static void physicsInit() {
    dInitODE();
    world = dWorldCreate();
    dWorldSetGravity(world, 0.0f, -9.8f, 0.0f);

    dMass mass;
    dMassSetZero(&mass);
    dMassSetSphereTotal(&mass, 1.0f, 0.5f);

    ball = dBodyCreate(world);
    dBodySetMass(ball, &mass);
    dBodySetPosition(ball, 0.0f, 50.0f, 0.0f);
}

static void physicsFree() {
    dWorldDestroy(world);
    dCloseODE();
}

static void physicsTick() {
    dWorldQuickStep(world, PHYSICS_TIMESTEP);

    const dReal* pos = dBodyGetPosition(ball);

    pthread_mutex_lock(&physicsMutex);
    ballPos[0] = pos[0];
    ballPos[1] = pos[1];
    ballPos[2] = pos[2];
    pthread_mutex_unlock(&physicsMutex);
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

static void* physicsThread(void* _) {
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

void initPhysics() {
    pthread_create(&thread, NULL, physicsThread, NULL);
}

void freePhysics() {
    shutdown = true;
    pthread_join(thread, NULL);
}
