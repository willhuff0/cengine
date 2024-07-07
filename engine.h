//
// Created by wehuf on 6/22/2024.
//

#ifndef ENGINE_H
#define ENGINE_H

#include "common.h"
#include "draw_queue.h"

typedef struct {
    vec4 viewPos;
    vec4 lightDir;
    vec4 lightIntensity;
} CEnginePbr;

typedef struct {
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    GLuint cengineUbo;
    GLuint cenginePbrUbo;
} Engine;

typedef struct {
    float time;
    float deltaTime;
    mat4 viewProjMat;
    CEnginePbr pbr;
    DrawQueue queue;
} FrameArgs;

extern Engine engine;
extern FrameArgs frameArgs;

void initEngine();

void engineLoop();

void freeEngine();

#endif //ENGINE_H
