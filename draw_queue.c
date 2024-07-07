//
// Created by Will on 7/6/24.
//

#include "draw_queue.h"

static void initCommand(DrawCommand* command) {
    command->ops = NULL;
}
static void freeCommand(DrawCommand* command) {
    arrfree(command->ops);
}

void initDrawQueue(DrawQueue* queue) {
    queue->commands = NULL;
    initCommand(&queue->workingCommand);
}
void freeDrawQueue(DrawQueue* queue) {
    for (int i = 0; i < arrlen(queue->commands); ++i) {
        freeCommand(&queue->commands[i]);
    }
    arrfree(queue->commands);
    freeCommand(&queue->workingCommand);
}

void drawQueueAddOp(DrawQueue* queue, DrawOp op) {
    arrput(queue->workingCommand.ops, op);
}
void drawQueueSubmit(DrawQueue* queue) {
    if (arrlen(queue->workingCommand.ops) == 0) return;
    arrput(queue->commands, queue->workingCommand);
    initCommand(&queue->workingCommand);
}

ShaderProgram* activeShader;

//
static void opBindSimpleMesh(DrawOp* op) {
    activeShader = op->obj.simpleMesh->material->shader;
    bindSimpleMesh(op->obj.simpleMesh);
}

static void opBindPbrMesh(DrawOp* op) {
    activeShader = op->obj.pbrMesh->material->shader;
    bindPbrMesh(op->obj.pbrMesh);
}

static void bindModelMat(TransComp* trans) {
    mat4 modelMat;
    glm_mat4_identity(modelMat);
    glm_scale(modelMat, trans->scale);
    glm_quat_rotate(modelMat, trans->rot, modelMat);
    glm_translate(modelMat, trans->rot);
    setUniformMat4(activeShader, "u_modelMat", modelMat);
}

static void opBindTrans(DrawOp* op) {
    if (activeShader == NULL) return;

    bindModelMat(op->obj.trans);
}

static void opBindPhysicsTrans(DrawOp* op) {
    if (activeShader == NULL) return;

    pthread_mutex_lock(&op->obj.physicsTrans->mutex);
    bindModelMat(&op->obj.physicsTrans->trans);
    pthread_mutex_unlock(&op->obj.physicsTrans->mutex);
}

static void opDrawSimpleMesh(DrawOp* op) {
    drawSimpleMesh(op->obj.simpleMesh);
}

static void opDrawPbrMesh(DrawOp* op) {
    drawPbrMesh(op->obj.pbrMesh);
}
//

typedef void (*OpExeFn)(DrawOp* op);

static OpExeFn exeFuncs[] = {
    [OP_BIND_SIMPLE_MESH] = opBindSimpleMesh,
    [OP_BIND_PBR_MESH] = opBindPbrMesh,
    [OP_BIND_TRANS] = opBindTrans,
    [OP_BIND_PHYSICS_TRANS] = opBindPhysicsTrans,
    [OP_DRAW_SIMPLE_MESH] = opDrawSimpleMesh,
    [OP_DRAW_PBR_MESH] = opDrawPbrMesh,
};

static void executeOp(DrawOp* op) {
    exeFuncs[op->type](op);
}

static void executeCommand(DrawCommand* command) {
    int priority = INT_MIN;

    for(;;) {
        DrawOpType lowest = INT_MAX;
        for (int i = 0; i < arrlen(command->ops); ++i) {
            DrawOp* op = &command->ops[i];
            if ((int)op->type < lowest && (int)op->type > priority) {
                lowest = (int)op->type;
            }
        }
        if (lowest == INT_MAX) break;
        priority = lowest;

        for (int i = 0; i < arrlen(command->ops); ++i) {
            DrawOp* op = &command->ops[i];
            if ((int)op->type == lowest) {
                executeOp(op);
            }
        }
    }

    activeShader = NULL;
}

void executeDrawQueue(DrawQueue* queue) {
    for (int i = 0; i < arrlen(queue->commands); ++i) {
        executeCommand(&queue->commands[i]);
    }
}