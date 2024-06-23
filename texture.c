//
// Created by wehuf on 6/22/2024.
//

#include "texture.h"

#include "scene.h"

bool createTexture(Texture** outTexture, const char* path) {
    int width, height, numChannels;
    uint8_t* data = stbi_load(path, &width, &height, &numChannels, 0);
    if (data == NULL) {
        fprintf(stderr, "[TEXTURE] Failed to load image: %s\n", path);
        return false;
    }

    GLuint gl_tex;
    glGenTextures(1, &gl_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gl_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    Texture* texture = arraddnptr(scene.textures, 1);
    texture->texture = gl_tex;
    if (outTexture != NULL) *outTexture = texture;
    return true;
}

void deleteTexture(Texture* texture) {
    glDeleteTextures(1, &texture->texture);
}

void bindTexture(Texture* texture, GLenum slot) {
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, texture->texture);
}