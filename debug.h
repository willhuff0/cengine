//
// Created by wehuf on 6/25/2024.
//

#ifndef DEBUG_H
#define DEBUG_H

#include <cglm/cglm.h>

void initDebug();
void freeDebug();

void debugDrawPoint(vec3 pos, float pixelSize, vec4 color);
void debugDrawVolume(vec3 center, vec3 extent, vec4 color);
void debugDrawSphere(vec3 pos, float radius, vec4 color);

void debugRenderFrame();

#endif //DEBUG_H
