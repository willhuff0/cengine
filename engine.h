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
} Engine;

typedef struct {
    float delta;
    mat4 viewProjMat;
} FrameInfo;

extern Engine engine;
extern FrameInfo frameInfo;

void initEngine();

void engineLoop();

void freeEngine();

#endif //ENGINE_H
