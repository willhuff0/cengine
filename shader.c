//
// Created by wehuf on 6/22/2024.
//

#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

#include "engine.h"
#include "scene.h"

static void printShaderInfoLog(GLuint shader, GLenum type) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    char* buffer = malloc(infoLogLength);
    glGetShaderInfoLog(shader, infoLogLength, NULL, buffer);

    fprintf(stderr, "[SHADER] Failed to compile %s shader: %s\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment", buffer);
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

static bool compileShader(GLuint shader, GLenum type) {
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        printShaderInfoLog(shader, type);
        return false;
    }

    return true;
}

static bool createShader(GLuint* shader, const char* path, GLenum type) {
    *shader = glCreateShader(type);

    char* source = readFile(path);
    glShaderSource(*shader, 1, &source, NULL);
    free(source);

    if (compileShader(*shader, type)) {
        return true;
    } else {
        glDeleteShader(*shader);
        return false;
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
    arrput(scene.shaders, sp);
    if (outSp != NULL) *outSp = sp;
    return true;
}

void deleteShaderProgram(ShaderProgram* sp) {
    glDeleteProgram(sp->program);
    free(sp);
}

void bindShaderProgram(ShaderProgram* sp) {
    glUseProgram(sp->program);
}