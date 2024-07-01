//
// Created by wehuf on 6/22/2024.
//

#ifndef SHADER_H
#define SHADER_H

#include "common.h"

#define DEFAULT_SHADER_SIMPLE_INSTANCED "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\simple_instanced.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\simple_instanced.frag"
#define DEFAULT_SHADER_POINTS_INSTANCED "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\points_instanced.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\simple_instanced.frag"
#define DEFAULT_SHADER_PBR "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\pbr.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\pbr.frag"

#define DEFAULT_SHADER_CUBEMAP "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\cubemap.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\cubemap.frag"

#define DEFAULT_SHADER_HDRI_TO_CUBEMAP "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\fullscreen.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\equirectangular_to_cubemap.frag"

typedef struct {
    char* key;
    int value;
} UniformNameLocation;

typedef struct {
    GLuint program;
    UniformNameLocation* uniformLocations;
} ShaderProgram;

bool createShaderProgram(ShaderProgram** sp, const char* vertPath, const char* fragPath);
void deleteShaderProgram(ShaderProgram* sp);

void bindShaderProgram(ShaderProgram* sp);
int getUniformLocation(ShaderProgram* sp, const char* name);

void setUniformBool(ShaderProgram* sp, const char* name, bool value);
void setUniformInt(ShaderProgram* sp, const char* name, int value);
void setUniformFloat(ShaderProgram* sp, const char* name, float value);
void setUniformVec2(ShaderProgram* sp, const char* name, vec2 value);
void setUniformVec3(ShaderProgram* sp, const char* name, vec3 value);
void setUniformVec4(ShaderProgram* sp, const char* name, vec4 value);
void setUniformMat4(ShaderProgram* sp, const char* name, mat4 value);

#endif //SHADER_H
