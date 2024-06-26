//
// Created by wehuf on 6/22/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

#define DEFAULT_ALBEDO_TEXTURE_PATH    "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\textures\\albedo_ao.png"
#define DEFAULT_NORMAL_TEXTURE_PATH    "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\textures\\normal.png"
#define DEFAULT_ROUGHNESS_TEXTURE_PATH "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\textures\\roughness.png"
#define DEFAULT_METALLIC_TEXTURE_PATH  "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\textures\\metallic.png"
#define DEFAULT_AO_TEXTURE_PATH        "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\textures\\albedo_ao.png"

typedef struct {
    GLuint texture;
} Texture;

bool createTexture(Texture** texture, const char* path);
void deleteTexture(Texture* texture);

void bindTexture(Texture* texture, GLuint slot);

#endif //TEXTURE_H
