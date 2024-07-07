//
// Created by wehuf on 6/22/2024.
//

#ifndef SHADER_H
#define SHADER_H

#include "common.h"

#define DEFAULT_SHADER_DIR DEFAULT_DIR "shaders" DIR

#define DEFAULT_SHADER_SIMPLE           DEFAULT_SHADER_DIR "simple.vert",           DEFAULT_SHADER_DIR "simple.frag"
#define DEFAULT_SHADER_SIMPLE_INSTANCED DEFAULT_SHADER_DIR "simple_instanced.vert", DEFAULT_SHADER_DIR "simple_instanced.frag"
#define DEFAULT_SHADER_POINTS_INSTANCED DEFAULT_SHADER_DIR "points_instanced.vert", DEFAULT_SHADER_DIR "simple_instanced.frag"
#define DEFAULT_SHADER_PBR              DEFAULT_SHADER_DIR "pbr.vert",              DEFAULT_SHADER_DIR "pbr.frag"

#define DEFAULT_SHADER_CUBEMAP          DEFAULT_SHADER_DIR "cubemap.vert", DEFAULT_SHADER_DIR "cubemap.frag"

#define DEFAULT_SHADER_HDRI_TO_CUBEMAP  DEFAULT_SHADER_DIR "equirectangular_to_cubemap.vert", DEFAULT_SHADER_DIR "equirectangular_to_cubemap.frag"

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
