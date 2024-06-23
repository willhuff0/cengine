//
// Created by wehuf on 6/22/2024.
//

#ifndef SHADER_H
#define SHADER_H

#include "common.h"

typedef struct {
    GLuint program;
} ShaderProgram;

bool createShaderProgram(ShaderProgram** sp, const char* vertPath, const char* fragPath);
void deleteShaderProgram(ShaderProgram* sp);

void bindShaderProgram(ShaderProgram* sp);

#endif //SHADER_H
