//
// Created by wehuf on 6/25/2024.
//

#ifndef DEBUG_H
#define DEBUG_H

#include <cglm/cglm.h>

void initDebug();
void freeDebug();

void debugDrawPixel(vec3 pos, float size = 3.0f);
void debugDrawSphere(vec3 pos, float radius = 1.0f);
void debugDrawVolume(vec3 center, vec3 extent);

#endif //DEBUG_H
