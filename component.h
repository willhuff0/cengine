//
// Created by Will on 7/6/24.
//

#ifndef COMPONENT_H
#define COMPONENT_H

#include "components/debug_draw_comp.h"
#include "components/mesh_comp.h"
#include "components/trans_comp.h"

#define ADD_COMP(iNode, iType, iAs, iComp) Comp comp; \
    comp.type = (iType); \
    comp.as.iAs = (iComp); \
    arrput((iNode)->components, comp);

enum CompType {
    COMP_TRANS,
    COMP_PHYSICS_TRANS,
    COMP_MESH,
    COMP_DEBUG_DRAW,
};

struct Comp {
    CompType type;
    union {
        TransComp* trans;
        PhysicsTransComp* physicsTrans;
        MeshComp* mesh;
        DebugDrawComp* debugDraw;
    } as;
};

void tickComp(Comp* comp);
void drawComp(Comp* comp);

void freeComp(Comp* comp);

#endif //COMPONENT_H
