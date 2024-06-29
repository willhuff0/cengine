//
// Created by wehuf on 6/22/2024.
//

#include "texture.h"

#include "scene.h"

static bool createTexture(Texture** outTexture, uint8_t* data, int width, int height, GLenum wrap, bool generateMipMaps) {
    GLuint gl_tex;
    glGenTextures(1, &gl_tex);
    glBindTexture(GL_TEXTURE_2D, gl_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMipMaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    if (generateMipMaps) glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    Texture* texture = malloc(sizeof(Texture));
    texture->texture = gl_tex;
    arrput(scene.textures, texture);
    if (outTexture != NULL) *outTexture = texture;
    return true;
}

bool createTextureFromPath(Texture** outTexture, const char* path) {
    int width, height, numChannels;
    uint8_t* data = stbi_load(path, &width, &height, &numChannels, 0);
    if (data == NULL) {
        fprintf(stderr, "[TEXTURE] Failed to load image from path: %s\n", path);
        stbi_image_free(data);
        return false;
    }

    return createTexture(outTexture, data, width, height, GL_REPEAT, true);
}

bool createTextureFromData(Texture** outTexture, const char* filename, uint8_t* buffer, unsigned int length) {
    int width, height, numChannels;
    uint8_t* data = stbi_load_from_memory(buffer, length, &width, &height, &numChannels, 0);
    if (data == NULL) {
        fprintf(stderr, "[TEXTURE] Failed to load from data: %s\n", filename);
        stbi_image_free(data);
        return false;
    }

    return createTexture(outTexture, data, width, height, GL_REPEAT, true);
}

// Order: px, nx, py, ny, pz, nz
bool createCubemapTextureFromPaths(Texture** outTexture, const char* paths[6]) {
    GLuint gl_tex;
    glGenTextures(1, &gl_tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_tex);

    for (unsigned int i = 0; i < 6; ++i) {
        const char* path = paths[i];

        int width, height, numChannels;
        uint8_t* data = stbi_load(path, &width, &height, &numChannels, 0);
        if (data == NULL) {
            fprintf(stderr, "[TEXTURE] Failed to load cubemap image from path: %s\n", path);
            stbi_image_free(data);
            return false;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Texture* texture = malloc(sizeof(Texture));
    texture->texture = gl_tex;
    arrput(scene.textures, texture);
    if (outTexture != NULL) *outTexture = texture;
    return true;
}

void createEmptyCubemapTexture(Texture** outTexture, int resolution) {
    assert(resolution > 0);

    GLuint gl_tex;
    glGenTextures(1, &gl_tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, gl_tex);

    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, resolution, resolution, 0, GL_RGB, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Texture* texture = malloc(sizeof(Texture));
    texture->texture = gl_tex;
    arrput(scene.textures, texture);
    if (outTexture != NULL) *outTexture = texture;
}

bool createHDRITextureFromPath(Texture** outTexture, const char* path) {
    int width, height, numChannels;
    stbi_set_flip_vertically_on_load(true);
    float* data = stbi_loadf(path, &width, &height, &numChannels, 0);
    stbi_set_flip_vertically_on_load(false);
    if (data == NULL) {
        fprintf(stderr, "[TEXTURE] Failed to load HDRI from path: %s\n", path);
        stbi_image_free(data);
        return false;
    }

    GLuint gl_tex;
    glGenTextures(1, &gl_tex);
    glBindTexture(GL_TEXTURE_2D, gl_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(data);

    Texture* texture = malloc(sizeof(Texture));
    texture->texture = gl_tex;
    arrput(scene.textures, texture);
    if (outTexture != NULL) *outTexture = texture;
    return true;
}

void deleteTexture(Texture* texture) {
    glDeleteTextures(1, &texture->texture);
    free(texture);
}

void bindTexture(Texture* texture, GLenum slot) {
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, texture->texture);
}