//
// Created by Will on 7/6/24.
//

#ifndef DRAW_QUEUE_H
#define DRAW_QUEUE_H

#include "pbr_mesh.h"
#include "simple_mesh.h"

#define DRAW_OP(iType, iObj, iOp) drawQueueAddOp(&frameArgs.queue, (DrawOp){.type = (iType), .obj.iObj = (iOp)})

typedef enum {
    OP_BIND_SIMPLE_MESH,
    OP_BIND_PBR_MESH,
    OP_BIND_TRANS,
    OP_BIND_PHYSICS_TRANS,
    OP_DRAW_SIMPLE_MESH,
    OP_DRAW_PBR_MESH,
} DrawOpType;

typedef struct {
    DrawOpType type;
    union {
        TransComp* trans;
        PhysicsTransComp* physicsTrans;
        SimpleMesh* simpleMesh;
        PbrMesh* pbrMesh;
    } obj;
} DrawOp;

typedef struct {
    DrawOp* ops;
} DrawCommand;

typedef struct {
    DrawCommand* commands;
    DrawCommand workingCommand;
} DrawQueue;

void initDrawQueue(DrawQueue* queue);
void freeDrawQueue(DrawQueue* queue);

void drawQueueAddOp(DrawQueue* queue, DrawOp op);
void drawQueueSubmit(DrawQueue* queue);

void executeDrawQueue(DrawQueue* queue);

#endif //DRAW_QUEUE_H
