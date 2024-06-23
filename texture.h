//
// Created by wehuf on 6/22/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

typedef struct {
    GLuint texture;
} Texture;

bool createTexture(Texture** texture, const char* path);
void deleteTexture(Texture* texture);

void bindTexture(Texture* texture, GLuint slot);

#endif //TEXTURE_H
