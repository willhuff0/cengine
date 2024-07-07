//
// Created by Will on 7/1/24.
//

#include "skybox.h"

#include "engine.h"
#include "scene.h"
#include "shader.h"

#define CUBEMAP_TEXTURE_SIZE 2048

static float cubeVertices[] = {
    // back face
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
     1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
    // front face
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
    // left face
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
    // right face
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
    // bottom face
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
     1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
    // top face
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
     1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
     1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
     1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
};

static float skyboxVertices[] = {
    // positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
};

static ShaderProgram* hdriToCubemapShader;
static GLuint fbo;
static GLuint rbo;
static GLuint cubeVao;
static GLuint cubeVbo;

static ShaderProgram* cubemapShader;
static GLuint cubemapVao;
static GLuint cubemapVbo;

void initSkybox() {
    glGenFramebuffers(1, &fbo);
    glGenRenderbuffers(1, &rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, CUBEMAP_TEXTURE_SIZE, CUBEMAP_TEXTURE_SIZE);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if (!createShaderProgram(&hdriToCubemapShader, DEFAULT_SHADER_HDRI_TO_CUBEMAP)) {
        fprintf(stderr, "[IBL] Failed to load HDRI to cubemap shader.\n");
        exit(48);
    }

    glGenVertexArrays(1, &cubeVao);
    glBindVertexArray(cubeVao);
    glGenBuffers(1, &cubeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    if (!createShaderProgram(&cubemapShader, DEFAULT_SHADER_CUBEMAP)) {
        fprintf(stderr, "[IBL] Failed to load cubemap shader.\n");
        exit(47);
    }
    bindShaderProgram(cubemapShader);
    glUniformBlockBinding(cubemapShader->program, glGetUniformBlockIndex(cubemapShader->program, "CEngine"), 0);

    glGenVertexArrays(1, &cubemapVao);
    glBindVertexArray(cubemapVao);
    glGenBuffers(1, &cubemapVbo);
    glBindBuffer(GL_ARRAY_BUFFER, cubemapVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vec3), 0);
}

void freeSkybox() {
    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(1, &rbo);

    glDeleteVertexArrays(1, &cubeVao);
    glDeleteBuffers(1, &cubeVbo);

    glDeleteVertexArrays(1, &cubemapVao);
    glDeleteBuffers(1, &cubemapVbo);
}

static Texture* hdriToCubemap(Texture* hdriTexture) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    Texture* cubemap;
    createEmptyCubemapTexture(&cubemap, CUBEMAP_TEXTURE_SIZE, false);

    bindShaderProgram(hdriToCubemapShader);
    bindTexture(hdriTexture, GL_TEXTURE0);
    setUniformInt(hdriToCubemapShader, "u_equirectangularMap", 0);

    mat4 projMat;
    glm_perspective(glm_rad(90.0f), 1.0f, 0.1f, 10.0f, projMat);
    mat4 viewMats[6];
    glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){ 1.0f,  0.0f,  0.0f}, (vec3){0.0f, -1.0f,  0.0f}, viewMats[0]);
    glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){-1.0f,  0.0f,  0.0f}, (vec3){0.0f, -1.0f,  0.0f}, viewMats[1]);
    glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){ 0.0f,  1.0f,  0.0f}, (vec3){0.0f,  0.0f,  1.0f}, viewMats[2]);
    glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){ 0.0f, -1.0f,  0.0f}, (vec3){0.0f,  0.0f, -1.0f}, viewMats[3]);
    glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){ 0.0f,  0.0f,  1.0f}, (vec3){0.0f, -1.0f,  0.0f}, viewMats[4]);
    glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){ 0.0f,  0.0f, -1.0f}, (vec3){0.0f, -1.0f,  0.0f}, viewMats[5]);

    setUniformMat4(hdriToCubemapShader, "u_projMat", projMat);

    glBindVertexArray(cubemapVao);

    glViewport(0, 0, CUBEMAP_TEXTURE_SIZE, CUBEMAP_TEXTURE_SIZE);
    for (unsigned int i = 0; i < 6; ++i) {
        setUniformMat4(hdriToCubemapShader, "u_viewMat", viewMats[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap->texture, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glViewport(0, 0, engine.windowWidth, engine.windowHeight);

    return cubemap;
}

void setupSceneSkybox(Texture* hdri) {
    scene.skybox.cubemap = hdriToCubemap(hdri);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, engine.windowWidth, engine.windowHeight);
}

void skyboxRenderFrame() {
    glDisable(GL_CULL_FACE);

    glBindVertexArray(cubemapVao);

    bindShaderProgram(cubemapShader);
    bindCubemapTexture(scene.skybox.cubemap, GL_TEXTURE0);
    setUniformInt(cubemapShader, "u_cubemap", 0);

    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
}