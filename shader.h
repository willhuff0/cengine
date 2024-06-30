//
// Created by wehuf on 6/22/2024.
//

#ifndef SHADER_H
#define SHADER_H

#include "common.h"

#define DEFAULT_SHADER_SIMPLE "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\simple.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\simple.frag"
#define DEFAULT_SHADER_SIMPLE_INSTANCED "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\simple_instanced.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\simple_instanced.frag"
#define DEFAULT_SHADER_POINTS_INSTANCED "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\points_instanced.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\simple_instanced.frag"
#define DEFAULT_SHADER_PBR "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\pbr.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\pbr.frag"

#define DEFAULT_SHADER_CUBEMAP "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\cubemap.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\cubemap.frag"

#define DEFAULT_SHADER_HDRI_TO_CUBEMAP "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\fullscreen.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\equirectangular_to_cubemap.frag"
#define DEFAULT_SHADER_IBL_FILTERING "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\fullscreen.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shaders\\ibl_filtering.frag"

typedef struct {
    GLuint program;
} ShaderProgram;

bool createShaderProgram(ShaderProgram** sp, const char* vertPath, const char* fragPath);
void deleteShaderProgram(ShaderProgram* sp);

void bindShaderProgram(ShaderProgram* sp);

#endif //SHADER_H
