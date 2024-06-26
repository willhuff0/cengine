//
// Created by wehuf on 6/25/2024.
//

#include "debug.h"

#include "common.h"
#include "model.h"
#include "simple_mesh.h"

typedef struct {
    vec3 pos;
    float size;
    vec3 color;
} PointDraw;

static ShaderProgram* pointsShader;
static GLuint pointsVao;
static GLuint pointsVbo;

static SimpleMesh* volumeMesh;
static SimpleMesh* sphereMesh;

void initDebug() {
    if (!createShaderProgram(&pointsShader, DEFAULT_SHADER_POINTS_INSTANCED)) {
        printf("[DEBUG] Failed to load default points instanced shader.\n");
        exit(98);
    }

    glGenVertexArrays(1, &pointsVao);
    glBindVertexArray(pointsVao);

    glGenBuffers(1, &pointsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PointDraw) * 1024, NULL, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(PointDraw), (void*)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, false, sizeof(PointDraw), (void*)offsetof(PointDraw, size));
    glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(PointDraw), (void*)offsetof(PointDraw, color));

    // ShaderProgram* shader;
    // if (!createShaderProgram(&shader, DEFAULT_SHADER_SIMPLE_INSTANCED)) {
    //     printf("[DEBUG] Failed to load default simple instanced shader.\n");
    //     exit(99);
    // }
    //
    // SimpleModel volumeModel;
    // loadSimpleModel(&volumeModel, shader, DEFAULT_MODEL_SHAPES_CUBE);
    // assert(volumeModel.numMeshes == 1);
    // volumeMesh = volumeModel.meshes[0];
    // freeSimpleModel(&volumeModel);
    //
    // SimpleModel sphereModel;
    // loadSimpleModel(&sphereModel, shader, DEFAULT_MODEL_SHAPES_ICOSPHERE_HIGHRES);
    // assert(sphereModel.numMeshes == 1);
    // sphereMesh = sphereModel.meshes[0];
    // freeSimpleModel(&sphereModel);
}

void freeDebug() {
    glDeleteVertexArrays(1, &pointsVao);
    glDeleteBuffers(1, &pointsVbo);
}

static PointDraw* pointDraws;

void debugDrawPoint(vec3 pos, float pixelSize, vec3 color) {
    PointDraw* pointDraw = arraddnptr(pointDraws, 1);
    glm_vec3_copy(pos, pointDraw->pos);
    pointDraw->size = pixelSize;
    glm_vec3_copy(color, pointDraw->color);
}

static void renderPoints() {
    if (pointDraws == NULL) return;

    bindShaderProgram(pointsShader);
    glBindVertexArray(pointsVao);

    int numPoints = arrlen(pointDraws);

    glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(PointDraw) * numPoints, pointDraws);

    glDrawArrays(GL_POINTS, 0, numPoints);

    arrfree(pointDraws);
    pointDraws = NULL;
}

void debugDrawSphere(vec3 pos, float radius, vec3 color) {

}

void debugDrawVolume(vec3 center, vec3 extent, vec3 color) {

}

void debugRenderFrame() {
    renderPoints();
}