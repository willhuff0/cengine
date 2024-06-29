//
// Created by wehuf on 6/23/2024.
//

#ifndef PBR_MESH_H
#define PBR_MESH_H

#include "common.h"
#include "pbr_material.h"

typedef struct {
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec2 uv;
} PbrVertex;

typedef struct {
    PbrMaterial* material;

    int numIndices;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} PbrMesh;

void createPbrMesh(PbrMesh** outMesh, PbrMaterial* material, unsigned int numVertices, PbrVertex* vertices, int numIndices, unsigned int* indices);
void deletePbrMesh(PbrMesh* mesh);

void bindPbrMesh(PbrMesh* mesh);
void drawPbrMesh(PbrMesh* mesh);

#endif //PBR_MESH_H
