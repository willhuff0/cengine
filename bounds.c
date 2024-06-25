//
// Created by wehuf on 6/25/2024.
//

#include "bounds.h"

void createBoundingBox(BoundingBox* box, vec3 firstPoint) {

}

void boundingBoxEncapsulate(BoundingBox* box, vec3 point);

void getBoundingBoxCenter(BoundingBox* box, vec3 center) {
    glm_vec3_lerp(box->min, box->max, 0.5f, center);
}

void getBoundingBoxExtent(BoundingBox* box, vec3 extent) {
    glm_vec3_sub(box->max, box->min, extent);
    glm_vec3_abs(extent, extent);
}