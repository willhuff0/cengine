//
// Created by wehuf on 6/22/2024.
//

#ifndef ENGINE_H
#define ENGINE_H

#include "common.h"

typedef struct {
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    GLuint cengineUbo;
} Engine;

typedef struct {
    float time;
    float deltaTime;
    mat4 viewProjMat;
} FrameArgs;

extern Engine engine;
extern FrameArgs frameArgs;

void initEngine();

void engineLoop();

void freeEngine();

#endif //ENGINE_H
