//
// Created by Will on 7/4/24.
//

#ifndef NODE_H
#define NODE_H

typedef enum CompType CompType;

typedef struct Comp Comp;

typedef struct {
    Comp* components;
} Node;

Node* createNode();
void deleteNode(Node* node);

void tickNode(Node* node);
void drawNode(Node* node);

Comp*  findComp(Node* node, CompType type);

#endif //NODE_H
