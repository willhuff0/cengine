//
// Created by Will on 7/6/24.
//

#ifndef DEBUG_DRAW_COMP_H
#define DEBUG_DRAW_COMP_H

#include <cglm/ivec4.h>

#include "../node.h"

typedef enum {
    DEBUG_POINT,
    DEBUG_SPHERE,
    DEBUG_VOLUME,
} DebugDrawType;

typedef struct {
    Node* node;
    DebugDrawType type;
    vec3 size;
    vec4 color;
} DebugDrawComp;

DebugDrawComp* addDebugDrawComp(Node* node);
void freeDebugDrawComp(DebugDrawComp* comp);

void tickDebugDrawComp(DebugDrawComp* comp);
void drawDebugDrawComp(DebugDrawComp* comp);

#endif //DEBUG_DRAW_COMP_H
