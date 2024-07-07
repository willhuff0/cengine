//
// Created by Will on 7/6/24.
//

#include "debug_draw_comp.h"

#include <stdlib.h>

#include "../component.h"
#include "../debug.h"

DebugDrawComp* addDebugDrawComp(Node* node) {
    DebugDrawComp* debugDraw = malloc(sizeof(DebugDrawComp));
    debugDraw->node = node;
    debugDraw->type = DEBUG_POINT;
    glm_vec3_one(debugDraw->size);
    glm_vec4_one(debugDraw->color);

    ADD_COMP(node, COMP_DEBUG_DRAW, debugDraw, debugDraw);
    return debugDraw;
}

void freeDebugDrawComp(DebugDrawComp* comp) {
    free(comp);
}

void tickDebugDrawComp(DebugDrawComp* comp) { }

static void drawDebugType(DebugDrawComp* debugDraw, TransComp* trans) {
    switch (debugDraw->type) {
        case DEBUG_POINT:
            debugDrawPoint(trans->pos, debugDraw->size[0], debugDraw->color);
            break;
        case DEBUG_SPHERE:
            debugDrawSphere(trans->pos, debugDraw->size[0], debugDraw->color);
            break;
        case DEBUG_VOLUME:
            debugDrawVolume(trans->pos, debugDraw->size, debugDraw->color);
            break;
    }
}

void drawDebugDrawComp(DebugDrawComp* debugDraw) {
    Comp* comp = findComp(debugDraw->node, COMP_TRANS);
    if (comp != NULL) {
        TransComp* trans = comp->as.trans;
        drawDebugType(debugDraw, trans);
        return;
    }

    comp = findComp(debugDraw->node, COMP_PHYSICS_TRANS);
    if (comp != NULL) {
        PhysicsTransComp* physicsTrans = comp->as.physicsTrans;
        pthread_mutex_lock(&physicsTrans->mutex);
        drawDebugType(debugDraw, &physicsTrans->trans);
        pthread_mutex_unlock(&physicsTrans->mutex);
        return;
    }
}