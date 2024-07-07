//
// Created by Will on 7/6/24.
//

#ifndef TRANS_COMP_H
#define TRANS_COMP_H

#include <pthread.h>

#include "../common.h"
#include "../node.h"

typedef struct {
    vec3 pos;
    versor rot;
    vec3 scale;
} TransComp;

typedef struct {
    pthread_mutex_t mutex;
    dBodyID body;
    TransComp trans;
} PhysicsTransComp;

TransComp* addTransComp(Node* node);
PhysicsTransComp* addPhysicsTransComp(Node* node);
void freeTransComp(TransComp* comp);
void freePhysicsTransComp(PhysicsTransComp* comp);

void tickTransComp(TransComp* comp);
void tickPhysicsTransComp(PhysicsTransComp* comp);
void drawTransComp(TransComp* comp);
void drawPhysicsTransComp(PhysicsTransComp* comp);

#endif //TRANS_COMP_H
