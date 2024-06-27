//
// Created by wehuf on 6/26/2024.
//

#include "ibl.h"

#include "engine.h"
#include "shader.h"
#include "texture.h"

#define HDRI_TO_CUBEMAP_RESOLUTION 2048
#define HDRI_TO_CUBEMAP_CONVOLUTION_RESOLUTION 32

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
     1.0f, -1.0f,  1.0f
};

static ShaderProgram* hdriToCubemapShader;
static ShaderProgram* convolutionShader;
static GLuint cubeVao;
static GLuint cubeVbo;

static ShaderProgram* cubemapShader;
static GLuint cubemapVao;
static GLuint cubemapVbo;

Texture* cubemapTexture;
Texture* irradianceTexture;
Texture* ggxLutTexture;

static void convertHDRIToCubemap(Texture* hdriTexture, Texture** outCubemapTexture, Texture** outIrradianceTexture) {
    GLuint captureFbo, captureRbo;
    glGenFramebuffers(1, &captureFbo);
    glGenRenderbuffers(1, &captureRbo);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFbo);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, HDRI_TO_CUBEMAP_RESOLUTION, HDRI_TO_CUBEMAP_RESOLUTION);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRbo);

    mat4 projMat;
    glm_perspective(glm_rad(90.0f), 1.0f, 0.1f, 10.0f, projMat);
    mat4 viewMats[6];
    glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){ 1.0f,  0.0f,  0.0f}, (vec3){0.0f, -1.0f,  0.0f}, viewMats[0]);
    glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){-1.0f,  0.0f,  0.0f}, (vec3){0.0f, -1.0f,  0.0f}, viewMats[1]);
    glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){ 0.0f,  1.0f,  0.0f}, (vec3){0.0f,  0.0f,  1.0f}, viewMats[2]);
    glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){ 0.0f, -1.0f,  0.0f}, (vec3){0.0f,  0.0f, -1.0f}, viewMats[3]);
    glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){ 0.0f,  0.0f,  1.0f}, (vec3){0.0f, -1.0f,  0.0f}, viewMats[4]);
    glm_lookat((vec3){0.0f, 0.0f, 0.0f}, (vec3){ 0.0f,  0.0f, -1.0f}, (vec3){0.0f, -1.0f,  0.0f}, viewMats[5]);

    Texture* cubemapTexture;
    createEmptyCubemapTexture(&cubemapTexture, HDRI_TO_CUBEMAP_RESOLUTION);

    bindShaderProgram(hdriToCubemapShader);
    glUniformMatrix4fv(0, 1, false, projMat);

    bindTexture(hdriTexture, GL_TEXTURE0);

    glViewport(0, 0, HDRI_TO_CUBEMAP_RESOLUTION, HDRI_TO_CUBEMAP_RESOLUTION);
    glBindVertexArray(cubeVao);
    for (unsigned int i = 0; i < 6; ++i) {
        glUniformMatrix4fv(1, 1, false, viewMats[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemapTexture->texture, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0 , 36);
    }

    // Convolution
    Texture* irradianceTexture;
    createEmptyCubemapTexture(&irradianceTexture, HDRI_TO_CUBEMAP_CONVOLUTION_RESOLUTION);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, HDRI_TO_CUBEMAP_CONVOLUTION_RESOLUTION, HDRI_TO_CUBEMAP_CONVOLUTION_RESOLUTION);

    bindShaderProgram(convolutionShader);
    glUniformMatrix4fv(0, 1, false, projMat);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture->texture);

    glViewport(0, 0, HDRI_TO_CUBEMAP_CONVOLUTION_RESOLUTION, HDRI_TO_CUBEMAP_CONVOLUTION_RESOLUTION);
    for (unsigned int i = 0; i < 6; ++i) {
        glUniformMatrix4fv(1, 1, false, viewMats[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceTexture->texture, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0 , 36);
    }

    glViewport(0, 0, engine.windowWidth, engine.windowHeight);
    glDeleteFramebuffers(1, &captureFbo);
    glDeleteRenderbuffers(1, &captureRbo);

    if (outCubemapTexture != NULL) *outCubemapTexture = cubemapTexture;
    if (outIrradianceTexture != NULL) *outIrradianceTexture = irradianceTexture;
}

void initIbl() {
    // HDRI to cubemap
    if (!createShaderProgram(&hdriToCubemapShader, DEFAULT_SHADER_HDRI_TO_CUBEMAP)) {
        fprintf(stderr, "[IBL] Failed to load HDRI to cubemap shader.\n");
        exit(45);
    }
    if (!createShaderProgram(&convolutionShader, DEFAULT_SHADER_CONVOLUTION)) {
        fprintf(stderr, "[IBL] Failed to load convolution shader.\n");
        exit(46);
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

    // Skybox
    if (!createShaderProgram(&cubemapShader, DEFAULT_SHADER_CUBEMAP)) {
        fprintf(stderr, "[IBL] Failed to load cubemap shader.\n");
        exit(47);
    }

    if (!createTextureFromPath(&ggxLutTexture, DEFAULT_TEXTURE_GGXLUT)) {
        fprintf(stderr, "[IBL] Failed to load GGX LUT texture.\n");
        exit(48);
    }

    // if (!createCubemapTextureFromPaths(&cubemapTexture, DEFAULT_TEXTURE_CUBEMAP_SKYBOX)) {
    //     fprintf(stderr, "[IBL] Failed to load skybox cubemap.\n");
    //     exit(49);
    // }
    Texture* hdriTexture;
    if (!createHDRITextureFromPath(&hdriTexture, DEFAULT_TEXTURE_HDRI_SKYBOX)) {
        fprintf(stderr, "[IBL] Failed to load skybox hdri.\n");
        exit(49);
    }
    convertHDRIToCubemap(hdriTexture, &cubemapTexture, &irradianceTexture);

    glGenVertexArrays(1, &cubemapVao);
    glBindVertexArray(cubemapVao);
    glGenBuffers(1, &cubemapVbo);
    glBindBuffer(GL_ARRAY_BUFFER, cubemapVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vec3), 0);
}

void iblRenderFrame() {
    glBindVertexArray(cubemapVao);
    bindShaderProgram(cubemapShader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture->texture);

    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}

void freeIbl() {
    glDeleteVertexArrays(1, &cubeVao);
    glDeleteBuffers(1, &cubeVbo);

    glDeleteVertexArrays(1, &cubemapVao);
    glDeleteBuffers(1, &cubemapVbo);
}
