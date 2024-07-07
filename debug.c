//
// Created by wehuf on 6/25/2024.
//

#include "debug.h"

#include "common.h"
#include "engine.h"
#include "model.h"
#include "simple_mesh.h"

typedef struct {
    vec3 pos;
    float size;
    vec4 color;
} PointDraw;

typedef struct {
    vec4 color;
    mat4 modelMat;
} SimpleDraw;

static ShaderProgram* pointsShader;
static GLuint pointsVao;
static GLuint pointsVbo;
static PointDraw* pointDraws;

static SimpleMesh* volumeMesh;
static SimpleDraw* volumeDraws;

static SimpleMesh* sphereMesh;
static SimpleDraw* sphereDraws;

void initDebug() {
    if (!createShaderProgram(&pointsShader, DEFAULT_SHADER_POINTS_INSTANCED)) {
        printf("[DEBUG] Failed to load default points instanced shader.\n");
        exit(98);
    }

    bindShaderProgram(pointsShader);
    glUniformBlockBinding(pointsShader->program, glGetUniformBlockIndex(pointsShader->program, "CEngine"), 0);

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
    glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(PointDraw), (void*)offsetof(PointDraw, color));

    ShaderProgram* shader;
    if (!createShaderProgram(&shader, DEFAULT_SHADER_SIMPLE_INSTANCED)) {
        printf("[DEBUG] Failed to load default simple instanced shader.\n");
        exit(99);
    }

    bindShaderProgram(shader);
    glUniformBlockBinding(shader->program, glGetUniformBlockIndex(shader->program, "CEngine"), 0);

    SimpleModel volumeModel;
    loadSimpleModel(&volumeModel, shader, DEFAULT_MODEL_SHAPES_CUBE);
    assert(volumeModel.numMeshes == 1);
    volumeMesh = volumeModel.meshes[0];
    freeSimpleModel(&volumeModel);

    SimpleModel sphereModel;
    loadSimpleModel(&sphereModel, shader, DEFAULT_MODEL_SHAPES_ICOSPHERE_HIGHRES);
    assert(sphereModel.numMeshes == 1);
    sphereMesh = sphereModel.meshes[0];
    freeSimpleModel(&sphereModel);
}

void freeDebug() {
    glDeleteVertexArrays(1, &pointsVao);
    glDeleteBuffers(1, &pointsVbo);
    arrfree(pointDraws);
    arrfree(volumeDraws);
    arrfree(sphereDraws);
}

void debugDrawPoint(vec3 pos, float pixelSize, vec4 color) {
    PointDraw* pointDraw = arraddnptr(pointDraws, 1);
    glm_vec3_copy(pos, pointDraw->pos);
    pointDraw->size = pixelSize;
    glm_vec4_copy(color, pointDraw->color);
}

static void renderPoints() {
    if (pointDraws == NULL) return;
    int numPoints = arrlen(pointDraws);
    if (numPoints < 1) return;

    bindShaderProgram(pointsShader);
    glBindVertexArray(pointsVao);

    glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(PointDraw) * numPoints, pointDraws);

    glDrawArrays(GL_POINTS, 0, numPoints);

    arrfree(pointDraws);
    pointDraws = NULL;
}

void debugDrawVolume(vec3 center, vec3 extent, vec4 color) {
    SimpleDraw* simpleDraw = arraddnptr(volumeDraws, 1);
    glm_vec4_copy(color, simpleDraw->color);
    glm_mat4_identity(simpleDraw->modelMat);
    glm_translate(simpleDraw->modelMat, center);
    glm_scale(simpleDraw->modelMat, extent);
}

static void renderVolumes() {
    if (volumeDraws == NULL) return;
    int numDraws = arrlen(volumeDraws);
    if (numDraws < 1) return;

    bindSimpleMesh(volumeMesh);

    GLuint instanceBuffer;
    glGenBuffers(1, &instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleDraw) * numDraws, volumeDraws, GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(SimpleDraw), 0);

    // mat4 columns
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(SimpleDraw), (void*)offsetof(SimpleDraw, modelMat));
    glVertexAttribPointer(4, 4, GL_FLOAT, false, sizeof(SimpleDraw), (void*)offsetof(SimpleDraw, modelMat) + sizeof(vec4));
    glVertexAttribPointer(5, 4, GL_FLOAT, false, sizeof(SimpleDraw), (void*)offsetof(SimpleDraw, modelMat) + sizeof(vec4) * 2);
    glVertexAttribPointer(6, 4, GL_FLOAT, false, sizeof(SimpleDraw), (void*)offsetof(SimpleDraw, modelMat) + sizeof(vec4) * 3);

    // Make instanced arrays
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    drawSimpleMeshInstanced(volumeMesh, numDraws);

    glDeleteBuffers(1, &instanceBuffer);

    arrfree(volumeDraws);
    volumeDraws = NULL;
}

void debugDrawSphere(vec3 pos, float radius, vec4 color) {
    SimpleDraw* simpleDraw = arraddnptr(sphereDraws, 1);
    glm_vec4_copy(color, simpleDraw->color);
    glm_mat4_identity(simpleDraw->modelMat);
    glm_translate(simpleDraw->modelMat, pos);
    glm_scale(simpleDraw->modelMat, (vec3){radius, radius, radius});
}

static void renderSpheres() {
    if (sphereDraws == NULL) return;
    int numDraws = arrlen(sphereDraws);
    if (numDraws < 1) return;

    bindSimpleMesh(sphereMesh);

    GLuint instanceBuffer;
    glGenBuffers(1, &instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleDraw) * numDraws, sphereDraws, GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(SimpleDraw), 0);

    // mat4 columns
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(SimpleDraw), (void*)offsetof(SimpleDraw, modelMat));
    glVertexAttribPointer(4, 4, GL_FLOAT, false, sizeof(SimpleDraw), (void*)offsetof(SimpleDraw, modelMat) + sizeof(vec4));
    glVertexAttribPointer(5, 4, GL_FLOAT, false, sizeof(SimpleDraw), (void*)offsetof(SimpleDraw, modelMat) + sizeof(vec4) * 2);
    glVertexAttribPointer(6, 4, GL_FLOAT, false, sizeof(SimpleDraw), (void*)offsetof(SimpleDraw, modelMat) + sizeof(vec4) * 3);

    // Make instanced arrays
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    drawSimpleMeshInstanced(sphereMesh, numDraws);

    glDeleteBuffers(1, &instanceBuffer);

    arrfree(sphereDraws);
    sphereDraws = NULL;
}

void debugRenderFrame() {
    renderVolumes();
    renderSpheres();
    renderPoints();
}