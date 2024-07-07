//
// Created by wehuf on 6/23/2024.
//

#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H

#include "shader.h"
#include "texture.h"

extern Texture* lightmap;

typedef struct {
    ShaderProgram* shader;

    vec4 albedo;
    float roughness;
    float metallic;

    Texture* albedoTex;
    Texture* normalTex;
    Texture* roughnessTex;
    Texture* metallicTex;
    Texture* aoTex;
} PbrMaterial;

PbrMaterial* createPbrMaterial(ShaderProgram* shader);
void bindPbrMaterial(const PbrMaterial* material);
void deletePbrMaterial(PbrMaterial* material);

#endif //PBR_MATERIAL_H
