//
// Created by wehuf on 6/23/2024.
//

#include "pbr_mesh.h"

#include "scene.h"

void createPbrMesh(PbrMesh** outMesh, PbrMaterial* material, unsigned int numVertices, PbrVertex* vertices, int numIndices, unsigned int* indices) {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(PbrVertex), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(PbrVertex), (void*)0);                              // Positions
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(PbrVertex), (void*)offsetof(PbrVertex, normal));    // Normals
    glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(PbrVertex), (void*)offsetof(PbrVertex, tangent));   // Tangents
    glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(PbrVertex), (void*)offsetof(PbrVertex, uv));        // UVs

    glBindVertexArray(0);

    PbrMesh* mesh = malloc(sizeof(PbrMesh));
    mesh->material = material;
    mesh->vao = vao;
    mesh->vbo = vbo;
    mesh->ebo = ebo;
    mesh->numIndices = numIndices;
    arrput(scene.pbrMeshes, mesh);
    if (outMesh != NULL) *outMesh = mesh;
}

void deletePbrMesh(PbrMesh* mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    free(mesh);
}

void bindPbrMesh(PbrMesh* mesh) {
    glBindVertexArray(mesh->vao);
    bindPbrMaterial(mesh->material);
}

void drawPbrMesh(PbrMesh* mesh) {
    glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, NULL);
}