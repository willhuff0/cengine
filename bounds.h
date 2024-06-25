//
// Created by wehuf on 6/25/2024.
//

#ifndef BOUNDS_H
#define BOUNDS_H

#include <cglm/cglm.h>

typedef struct {
    vec3 min;
    vec3 max;
} BoundingBox;

void createBoundingBox(BoundingBox* box, vec3 firstPoint);
void boundingBoxEncapsulate(BoundingBox* box, vec3 point);

void getBoundingBoxCenter(BoundingBox* box, vec3 center);
void getBoundingBoxExtent(BoundingBox* box, vec3 extent);

#endif //BOUNDS_H
