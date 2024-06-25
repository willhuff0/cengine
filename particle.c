//
// Created by wehuf on 6/24/2024.
//

#include "particle.h"

#include <time.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cglm/cglm.h>
#include <GLES3/gl32.h>

#include "engine.h"
#include "mesh.h"
#include "shader.h"

#define NUM_PARTICLES 1500

#define BOUNDING_SPHERE_RADIUS 10.0f
#define BOUNDING_SPHERE_SPAWN_AREA (2.0f * BOUNDING_SPHERE_RADIUS) / 1.73205080757f // sqrtf(3.0f)

#define PARTICLE_RADIUS 0.25f
#define PARTICLE_GRAVITY 9.8f

typedef struct {
    vec3 previous;
    vec3 current;
} Particle;

static vec3 particleColors[NUM_PARTICLES];

static Particle particles[NUM_PARTICLES];
static GLuint vao;
static GLuint vbo;
static GLuint ebo;

static float randFloat(float min, float max) {
    return min + (rand() / (float) RAND_MAX) * ( max - min );
}

static void initParticle(Particle* particle) {
    float x = randFloat(-BOUNDING_SPHERE_SPAWN_AREA, BOUNDING_SPHERE_SPAWN_AREA);
    float y = randFloat(-BOUNDING_SPHERE_SPAWN_AREA, BOUNDING_SPHERE_SPAWN_AREA);
    float z = randFloat(-BOUNDING_SPHERE_SPAWN_AREA, BOUNDING_SPHERE_SPAWN_AREA);
    glm_vec3_copy((vec3){x, y, z}, particle->current);
    glm_vec3_copy((vec3){x, y, z}, particle->previous);
}

static ShaderProgram* particlesShader;
static ShaderProgram* pointsShader;

static int vertexCount;
static int indexCount;

void initParticles() {
    srand(time(NULL));

    for (int i = 0; i < NUM_PARTICLES; ++i) {
        particleColors[i][0] = randFloat(0.0f, 1.0f);
        particleColors[i][1] = randFloat(0.0f, 1.0f);
        particleColors[i][2] = randFloat(0.0f, 1.0f);
        initParticle(&particles[i]);
    }

    if (!createShaderProgram(&particlesShader, "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\particle.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\particle.frag")) {
        fprintf(stderr, "Failed to create shader asset.");
        exit(88);
    }
    if (!createShaderProgram(&pointsShader, "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\point.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\point.frag")) {
        fprintf(stderr, "Failed to create shader asset.");
        exit(88);
    }
    bindShaderProgram(particlesShader);

    const struct aiScene* aiScene = aiImportFile("C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\icosphere.obj",
                                            aiProcess_Triangulate |
                                            aiProcess_PreTransformVertices |
                                            aiProcess_JoinIdenticalVertices |
                                            aiProcess_FlipUVs |
                                            aiProcess_CalcTangentSpace |
                                            aiProcess_GenNormals);

    const struct aiMesh* aiMesh = aiScene->mMeshes[0];

    vertexCount = aiMesh->mNumVertices;
    vec3* vertices = malloc(vertexCount * sizeof(vec3));
    indexCount = aiMesh->mNumFaces * 3;
    unsigned int* indices = malloc(indexCount * sizeof(unsigned int));

    for (unsigned int i = 0; i < vertexCount; ++i) {
        vertices[i][0] = aiMesh->mVertices[i].x * PARTICLE_RADIUS;
        vertices[i][1] = aiMesh->mVertices[i].y * PARTICLE_RADIUS;
        vertices[i][2] = aiMesh->mVertices[i].z * PARTICLE_RADIUS;
    }

    for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i) {
        struct aiFace face = aiMesh->mFaces[i];
        indices[i * 3 + 0] = face.mIndices[0];
        indices[i * 3 + 1] = face.mIndices[1];
        indices[i * 3 + 2] = face.mIndices[2];
    }

    // Particles
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(vec3), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

    free(vertices);
    free(indices);
}

static void applyForce(vec3 acceleration, Particle* particle, vec3 center, float strength) {
    vec3 disp;
    glm_vec3_sub(particle->current, center, disp);
    float dist = glm_vec3_distance((vec3){0.0f, 0.0f, 0.0f}, disp);
    if (disp > 0) {
        vec3 norm;
        glm_vec3_divs(disp, dist, norm);
        glm_vec3_scale(norm, strength, norm);
        glm_vec3_add(acceleration, norm, acceleration);
    }
}

static void handleCollision(Particle* a, Particle* b, float dist2) {
    vec3 axis;
    glm_vec3_sub(a->current, b->current, axis);

    float dist = sqrtf(dist2);

    vec3 norm;
    glm_vec3_divs(axis, dist, norm);
    float delta = 2.0f * PARTICLE_RADIUS - dist;
    glm_vec3_scale(norm, 0.5f * delta, norm);
    glm_vec3_add(a->current, norm, a->current);
    glm_vec3_sub(b->current, norm, b->current);
}

static void checkCollisions(Particle* particle, int particleIndex) {
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        if (i == particleIndex) continue;

        Particle* otherParticle = &particles[i];
        float dist2 = glm_vec3_distance2(particle->current, otherParticle->current);
        if (dist2 < glm_pow2(PARTICLE_RADIUS * 2.0f)) {
            handleCollision(particle, otherParticle, dist2);
        }
    }
}

static void tickParticle(Particle* particle, float delta) {
    vec3 acceleration = {0.0f, -PARTICLE_GRAVITY, 0.0f};
    if (glfwGetKey(engine.window, GLFW_KEY_F) == GLFW_PRESS) applyForce(acceleration, particle, (vec3){-10.0f, -10.0f, 0.0f}, 20.0f);

    vec3 disp;
    glm_vec3_sub(particle->current, particle->previous, disp);
    glm_vec3_copy(particle->current, particle->previous);
    glm_vec3_scale(acceleration, delta * delta, acceleration);
    glm_vec3_add(particle->current, disp, particle->current);
    glm_vec3_add(particle->current, acceleration, particle->current);

    float distFromCenter = sqrtf(glm_pow2(particle->current[0]) + glm_pow2(particle->current[1]) + glm_pow2(particle->current[2]));
    float distFromBoundingSphere = distFromCenter - BOUNDING_SPHERE_RADIUS;
    if (distFromBoundingSphere > 0.0f) {
        vec3 unitVec;
        glm_normalize_to(particle->current, unitVec);
        glm_vec3_scale(unitVec, BOUNDING_SPHERE_RADIUS, particle->current);
    }
}

void tickParticles(float delta) {
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        tickParticle(&particles[i], delta);
        checkCollisions(&particles[i], i);
    }
}

void drawParticles() {
    // Particles
    bindShaderProgram(particlesShader);
    glUniformMatrix4fv(0, 1, false, frameInfo.viewProjMat);

    glBindVertexArray(vao);

    vec3 positions[NUM_PARTICLES];
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        glm_vec3_copy(particles[i].current, positions[i]);
    }
    glUniform3fv(1, NUM_PARTICLES, positions);
    glUniform3fv(NUM_PARTICLES + 1, NUM_PARTICLES, particleColors);

    glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL, NUM_PARTICLES);

    // Points
    bindShaderProgram(pointsShader);
    glUniformMatrix4fv(0, 1, false, frameInfo.viewProjMat);

    mat4 model;
    glm_mat4_identity(model);
    glm_scale(model, (vec3){BOUNDING_SPHERE_RADIUS * 4.0f, BOUNDING_SPHERE_RADIUS * 4.0f, BOUNDING_SPHERE_RADIUS * 4.0f});
    glUniformMatrix4fv(1, 1, false, model);

    glUniform3f(2, 0.5f, 0.5f, 0.5f);

    glDrawArrays(GL_POINTS, 0, vertexCount);
}

void freeParticles() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}