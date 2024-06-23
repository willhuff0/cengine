//
// Created by wehuf on 6/23/2024.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"

typedef struct {
    Texture* albedo;
    Texture* normal;
    Texture* roughness;
    Texture* metallic;
    Texture* ao;
} Material;

void initMaterial(Material* material);
void bindMaterial(const Material* material);

#endif //MATERIAL_H
