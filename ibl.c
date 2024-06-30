//
// Created by wehuf on 6/26/2024.
//

#include "ibl.h"

#include "engine.h"
#include "shader.h"
#include "texture.h"

#define TEXTURE_SIZE 256
#define GGX_SAMPLE_COUNT 1024
#define LAMBERTIAN_SAMPLE_COUNT 2048
#define SHEEN_SAMPLE_COUNT 64
#define LOD_BIAS 0.0f
#define LOWEST_MIP_LEVEL 4
#define LUT_RESOLUTION 1024

ShaderProgram* hdriToCubemapShader;
ShaderProgram* filteringShader;

void initIbl() {
    if (!createShaderProgram(&hdriToCubemapShader, DEFAULT_SHADER_HDRI_TO_CUBEMAP)) {
        fprintf(stderr, "[IBL] Failed to load HDRI to cubemap shader.\n");
        exit(48);
    }
    if (!createShaderProgram(&filteringShader, DEFAULT_SHADER_IBL_FILTERING)) {
        fprintf(stderr, "[IBL] Failed to load filtering shader.\n");
        exit(49);
    }
}

void freeIbl() {

}

static void hdriToCubemap(IBL* ibl, GLuint fbo, Texture* hdriTexture) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    for (int i = 0; i < 6; ++i) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, ibl->cubemapTexture->texture, 0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, ibl->cubemapTexture->texture);

        glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);

        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bindShaderProgram(hdriToCubemapShader);

        bindTexture(hdriTexture, GL_TEXTURE0);

        glUniform1i(0, i);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, ibl->cubemapTexture->texture);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

static void applyFilter(IBL* ibl, GLuint fbo, int distribution, float roughness, int targetMipLevel, GLuint targetTexture, int sampleCount, float lodBias) {
    int currentTextureSize = TEXTURE_SIZE >> targetMipLevel;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    for (int i = 0; i < 6; ++i) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, targetTexture, targetMipLevel);

        glBindTexture(GL_TEXTURE_CUBE_MAP, targetTexture);

        glViewport(0, 0, currentTextureSize, currentTextureSize);

        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bindShaderProgram(filteringShader);

        bindCubemapTexture(ibl->cubemapTexture, GL_TEXTURE0);

        glUniform1f(0, roughness);
        glUniform1i(1, sampleCount);
        glUniform1i(2, TEXTURE_SIZE);
        glUniform1f(3, lodBias);
        glUniform1i(4, distribution);
        glUniform1i(5, i);
        glUniform1i(6, 0);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

static void cubemapToLambertian(IBL* ibl, GLuint fbo) {
    applyFilter(ibl, fbo, 0, 0.0f, 0, ibl->lambertianTexture->texture, LAMBERTIAN_SAMPLE_COUNT, 0.0f);
}

static void cubemapToGGX(IBL* ibl, GLuint fbo) {
    for (int currentMipLevel = 0; currentMipLevel <= ibl->mipmapLevels; ++currentMipLevel) {
        float roughness = (float)currentMipLevel / (float)(ibl->mipmapLevels - 1);
        applyFilter(ibl, fbo, 1, roughness, currentMipLevel, ibl->ggxTexture->texture, GGX_SAMPLE_COUNT, 0.0f);
    }
}

static void cubemapToSheen(IBL* ibl, GLuint fbo) {
    for (int currentMipLevel = 0; currentMipLevel <= ibl->mipmapLevels; ++currentMipLevel) {
        float roughness = (float)currentMipLevel / (float)(ibl->mipmapLevels - 1);
        applyFilter(ibl, fbo, 2, roughness, currentMipLevel, ibl->sheenTexture->texture, SHEEN_SAMPLE_COUNT, 0.0f);
    }
}

static void sampleLut(IBL* ibl, GLuint fbo, int distribution, GLuint targetTexture, int currentTextureSize) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, targetTexture, 0);

    glBindTexture(GL_TEXTURE_2D, targetTexture);

    glViewport(0, 0, currentTextureSize, currentTextureSize);

    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    bindShaderProgram(filteringShader);

    bindCubemapTexture(ibl->cubemapTexture, GL_TEXTURE0);

    glUniform1f(0, 0.0f);
    glUniform1i(1, 512);
    glUniform1i(2, 0);
    glUniform1f(3, 0.0f);
    glUniform1i(4, distribution);
    glUniform1i(5, 0);
    glUniform1i(6, 1);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void sampleGGXLut(IBL* ibl, GLuint fbo) {
    createEmptyLutTexture(&ibl->ggxLutTexture, LUT_RESOLUTION);
    sampleLut(ibl, fbo, 1, ibl->ggxLutTexture->texture, LUT_RESOLUTION);
}

static void sampleCharlieLut(IBL* ibl, GLuint fbo) {
    createEmptyLutTexture(&ibl->charlieLutTexture, LUT_RESOLUTION);
    sampleLut(ibl, fbo, 2, ibl->charlieLutTexture->texture, LUT_RESOLUTION);
}

void setupSceneIbl(IBL* ibl, Texture* hdriTexture) {
    createEmptyCubemapTexture(&ibl->cubemapTexture, TEXTURE_SIZE, true);

    GLuint fbo;
    glGenFramebuffers(1, &fbo);

    createEmptyCubemapTexture(&ibl->lambertianTexture, TEXTURE_SIZE, false);
    createEmptyCubemapTexture(&ibl->ggxTexture, TEXTURE_SIZE, true);
    createEmptyCubemapTexture(&ibl->sheenTexture, TEXTURE_SIZE, true);

    glBindTexture(GL_TEXTURE_CUBE_MAP, ibl->ggxTexture->texture);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindTexture(GL_TEXTURE_CUBE_MAP, ibl->sheenTexture->texture);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    ibl->mipmapLevels = floor(log2(TEXTURE_SIZE)) + 1 - LOWEST_MIP_LEVEL;

    // Filter all
    hdriToCubemap(ibl, fbo, hdriTexture);
    cubemapToLambertian(ibl, fbo);
    cubemapToGGX(ibl, fbo);
    cubemapToSheen(ibl, fbo);

    sampleGGXLut(ibl, fbo);
    sampleCharlieLut(ibl, fbo);

    glDeleteFramebuffers(1, &fbo);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, engine.windowWidth, engine.windowHeight);
}
