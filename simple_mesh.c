//
// Created by wehuf on 6/23/2024.
//

#include "simple_mesh.h"

#include "scene.h"

void createSimpleMesh(SimpleMesh** outMesh, SimpleMaterial* material, int64_t numVertices, SimpleVertex* vertices, int64_t numIndices, unsigned int* indices) {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(SimpleVertex), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(SimpleVertex), (void*)0);                                 // Positions
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(SimpleVertex), (void*)offsetof(SimpleVertex, normal));    // Normals

    SimpleMesh* mesh = malloc(sizeof(SimpleMesh));
    mesh->material = material;
    mesh->vao = vao;
    mesh->vbo = vbo;
    mesh->ebo = ebo;
    mesh->numIndices = numIndices;
    arrput(scene.simpleMeshes, mesh);
    if (outMesh != NULL) *outMesh = mesh;
}

void deleteSimpleMesh(SimpleMesh* mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    free(mesh);
}

void bindSimpleMesh(SimpleMesh* mesh) {
    glBindVertexArray(mesh->vao);
    bindSimpleMaterial(mesh->material);
}

void drawSimpleMesh(SimpleMesh* mesh) {
    glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, NULL);
}

void drawSimpleMeshInstanced(SimpleMesh* mesh, int numInstances) {
    glDrawElementsInstanced(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, NULL, numInstances);
}