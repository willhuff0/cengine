//
// Created by wehuf on 6/23/2024.
//

#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H

#include "shader.h"
#include "texture.h"

extern Texture* lightmapTexture;

typedef struct {
    ShaderProgram* shader;

    Texture* albedo;
    Texture* normal;
    Texture* roughness;
    Texture* metallic;
    Texture* ao;
} PbrMaterial;

PbrMaterial* createPbrMaterial(ShaderProgram* shader);
void bindPbrMaterial(const PbrMaterial* material);
void deletePbrMaterial(PbrMaterial* material);

#endif //PBR_MATERIAL_H
