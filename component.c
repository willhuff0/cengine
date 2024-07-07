//
// Created by Will on 7/6/24.
//

#include "component.h"

// typedef void (*CompFn)(void* obj);
//
// typedef struct {
//     CompFn tick;
//     CompFn draw;
//     CompFn free;
// } CompFns;
//
// static CompFns funcs[] = {
//     [COMP_TRANS]         = {tickTransComp,        drawTransComp,        freeTransComp},
//     [COMP_PHYSICS_TRANS] = {tickPhysicsTransComp, drawPhysicsTransComp, freePhysicsTransComp},
//     [COMP_MESH]          = {tickMeshComp,         drawMeshComp,         freeMeshComp},
//     [COMP_DEBUG_DRAW]    = {tickDebugDrawComp,    drawDebugDrawComp,    freeDebugDrawComp},
// };

void tickComp(Comp* comp) {
    switch (comp->type) {
        case COMP_TRANS:
            tickTransComp(comp->as.trans);
            break;
        case COMP_PHYSICS_TRANS:
            tickPhysicsTransComp(comp->as.physicsTrans);
            break;
        case COMP_MESH:
            tickMeshComp(comp->as.mesh);
            break;
        case COMP_DEBUG_DRAW:
            tickDebugDrawComp(comp->as.debugDraw);
            break;
    }
}
void drawComp(Comp* comp) {
    switch (comp->type) {
        case COMP_TRANS:
            drawTransComp(comp->as.trans);
            break;
        case COMP_PHYSICS_TRANS:
            drawPhysicsTransComp(comp->as.physicsTrans);
            break;
        case COMP_MESH:
            drawMeshComp(comp->as.mesh);
            break;
        case COMP_DEBUG_DRAW:
            drawDebugDrawComp(comp->as.debugDraw);
            break;
    }
}

void freeComp(Comp* comp) {
    switch (comp->type) {
        case COMP_TRANS:
            freeTransComp(comp->as.trans);
            break;
        case COMP_PHYSICS_TRANS:
            freePhysicsTransComp(comp->as.physicsTrans);
            break;
        case COMP_MESH:
            freeMeshComp(comp->as.mesh);
            break;
        case COMP_DEBUG_DRAW:
            freeDebugDrawComp(comp->as.debugDraw);
            break;
    }
}