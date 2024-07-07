//
// Created by Will on 7/6/24.
//

#include "trans_comp.h"

#include "../component.h"
#include "../draw_queue.h"
#include "../engine.h"

static void initTrans(TransComp* trans) {
    glm_vec3_zero(trans->pos);
    glm_quat_identity(trans->pos);
    glm_vec3_one(trans->scale);
}

TransComp* addTransComp(Node* node) {
    TransComp* trans = malloc(sizeof(TransComp));
    initTrans(trans);

    ADD_COMP(node, COMP_TRANS, trans, trans)
    return trans;
}

PhysicsTransComp* addPhysicsTransComp(Node* node) {
    PhysicsTransComp* physicsTrans = malloc(sizeof(PhysicsTransComp));
    pthread_mutex_init(&physicsTrans->mutex, NULL);
    physicsTrans->body = NULL;
    // QUEUE PHYSICS ADD BODY
    initTrans(&physicsTrans->trans);

    ADD_COMP(node, COMP_PHYSICS_TRANS, physicsTrans, physicsTrans)
    return physicsTrans;
}

void freeTransComp(TransComp* comp) {
    free(comp);
}

void freePhysicsTransComp(PhysicsTransComp* comp) {
    pthread_mutex_destroy(&comp->mutex);
    // QUEUE PHYSICS REMOVE BODY
    free(comp);
}

void tickTransComp(TransComp* comp) {}
void tickPhysicsTransComp(PhysicsTransComp* comp) {}

void drawTransComp(TransComp* comp) {
    DRAW_OP(OP_BIND_TRANS, trans, comp);
}

void drawPhysicsTransComp(PhysicsTransComp* comp) {
    DRAW_OP(OP_BIND_PHYSICS_TRANS, physicsTrans, comp);
}