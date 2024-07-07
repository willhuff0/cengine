//
// Created by Will on 7/4/24.
//

#include "node.h"

#include "component.h"
#include "draw_queue.h"
#include "engine.h"
#include "physics.h"
#include "scene.h"

Node* createNode() {
    Node* node = malloc(sizeof(Node));
    node->components = NULL;
    arrput(scene.nodes, node);
    return node;
}

void deleteNode(Node* node) {
    for (int i = 0; i < arrlen(node->components); ++i) {
        freeComp(&node->components[i]);
    }
    free(node);
}

void tickNode(Node* node) {
    for (int i = 0; i < arrlen(node->components); ++i) {
        tickComp(&node->components[i]);
    }
}

void drawNode(Node* node) {
    for (int i = 0; i < arrlen(node->components); ++i) {
        drawComp(&node->components[i]);
    }
    drawQueueSubmit(&frameArgs.queue);
}

Comp* findComp(Node* node, CompType type) {
    for (int i = 0; i < arrlen(node->components); ++i) {
        Comp* comp = &node->components[i];
        if (comp->type == type) {
            return comp;
        }
    }

    return NULL;
}