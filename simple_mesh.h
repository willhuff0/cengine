//
// Created by wehuf on 6/23/2024.
//

#ifndef MESH_H
#define MESH_H

#include "common.h"
#include "material.h"

typedef struct {
    vec3 position;
    vec3 normal;
} Vertex;

typedef struct {
    Material* material;

    int numIndices;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} Mesh;

void createMesh(Mesh** mesh, Material material, int64_t numVertices, Vertex* vertices, int64_t numIndices, unsigned int* indices);
void deleteMesh(Mesh* mesh);

void bindMesh(Mesh* mesh);
void drawMesh(Mesh* mesh);

#endif //MESH_H
