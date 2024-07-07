//
// Created by wehuf on 6/22/2024.
//

#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

#include "engine.h"
#include "scene.h"

static void printShaderInfoLog(GLuint shader, GLenum type, const char* path) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    char* buffer = malloc(infoLogLength);
    glGetShaderInfoLog(shader, infoLogLength, NULL, buffer);

    fprintf(stderr, "[SHADER] Failed to compile %s shader (%s): %s\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment", path, buffer);
    free(buffer);
}

static void printProgramInfoLog(GLuint program) {
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    char* buffer = malloc(infoLogLength);
    glGetProgramInfoLog(program, infoLogLength, NULL, buffer);

    fprintf(stderr, "[SHADER] Failed to link program: %s\n", buffer);
    free(buffer);
}

static bool compileShader(GLuint shader, GLenum type, const char* path) {
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        printShaderInfoLog(shader, type, path);
        return false;
    }

    return true;
}

static bool createShader(GLuint* shader, const char* path, GLenum type) {
    *shader = glCreateShader(type);

    char* source = readFile(path);
    glShaderSource(*shader, 1, &source, NULL);
    free(source);

    if (compileShader(*shader, type, path)) {
        return true;
    } else {
        glDeleteShader(*shader);
        return false;
    }
}

static void lookupShaderUniformLocations(ShaderProgram* sp) {
    int numUniforms;
    glGetProgramiv(sp->program, GL_ACTIVE_UNIFORMS, &numUniforms);

    for (unsigned int i = 0; i < numUniforms; ++i) {
        int nameLength;
        glGetActiveUniformsiv(sp->program, 1, &i, GL_UNIFORM_NAME_LENGTH, &nameLength);

        char* nameBuffer = malloc(nameLength * sizeof(char));
        int length, size;
        GLuint type;
        glGetActiveUniform(sp->program, i, nameLength, &length, &size, &type, nameBuffer);
        int location = glGetUniformLocation(sp->program, nameBuffer);
        shput(sp->uniformLocations, nameBuffer, location);
    }
}

bool createShaderProgram(ShaderProgram** outSp, const char* vertPath, const char* fragPath) {
    GLuint vertShader;
    if (!createShader(&vertShader, vertPath, GL_VERTEX_SHADER)) {
        return false;
    }

    GLuint fragShader;
    if (!createShader(&fragShader, fragPath, GL_FRAGMENT_SHADER)) {
        glDeleteShader(vertShader);
        return false;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    glDetachShader(program, vertShader);
    glDetachShader(program, fragShader);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        printProgramInfoLog(program);
        glDeleteProgram(program);
        return false;
    }

    ShaderProgram* sp = malloc(sizeof(ShaderProgram));
    sp->program = program;
    sp->uniformLocations = NULL;

    shdefault(sp->uniformLocations, -1);
    lookupShaderUniformLocations(sp);

    arrput(scene.shaders, sp);
    if (outSp != NULL) *outSp = sp;
    return true;
}

void deleteShaderProgram(ShaderProgram* sp) {
    glDeleteProgram(sp->program);
    shfree(sp->uniformLocations);
    free(sp);
}

void bindShaderProgram(ShaderProgram* sp) {
    glUseProgram(sp->program);
}

int getUniformLocation(ShaderProgram* sp, const char* name) {
    int location = shget(sp->uniformLocations, name);
    assert(location != -1); // -1: uniform does not exist
    return location;
}

void setUniformBool(ShaderProgram* sp, const char* name, bool value) {
    setUniformInt(sp, name, value ? 1 : 0);
}
void setUniformInt(ShaderProgram* sp, const char* name, int value) {
    glUniform1i(getUniformLocation(sp, name), value);
}
void setUniformFloat(ShaderProgram* sp, const char* name, float value) {
    glUniform1f(getUniformLocation(sp, name), value);
}
void setUniformVec2(ShaderProgram* sp, const char* name, vec2 value) {
    glUniform2f(getUniformLocation(sp, name), value[0], value[1]);
}
void setUniformVec3(ShaderProgram* sp, const char* name, vec3 value) {
    glUniform3f(getUniformLocation(sp, name), value[0], value[1], value[2]);
}
void setUniformVec4(ShaderProgram* sp, const char* name, vec4 value) {
    glUniform4f(getUniformLocation(sp, name), value[0], value[1], value[2], value[3]);
}
void setUniformMat4(ShaderProgram* sp, const char* name, mat4 value) {
    glUniformMatrix4fv(getUniformLocation(sp, name), 1, false, value);
}