//
// Created by wehuf on 6/23/2024.
//

#ifndef SIMPLE_MESH_H
#define SIMPLE_MESH_H

#include "common.h"
#include "simple_material.h"

typedef struct {
    vec3 position;
    vec3 normal;
} SimpleVertex;

typedef struct {
    SimpleMaterial* material;

    int64_t numIndices;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} SimpleMesh;

void createSimpleMesh(SimpleMesh** mesh, SimpleMaterial* material, int64_t numVertices, SimpleVertex* vertices, int64_t numIndices, unsigned int* indices);
void deleteSimpleMesh(SimpleMesh* mesh);

void bindSimpleMesh(SimpleMesh* mesh);
void drawSimpleMesh(SimpleMesh* mesh);
void drawSimpleMeshInstanced(SimpleMesh* mesh, int numInstances);

#endif //SIMPLE_MESH_H
