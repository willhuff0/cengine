//
// Created by wehuf on 6/23/2024.
//

#include "pbr_material.h"

#include "engine.h"
#include "ibl.h"
#include "scene.h"

Texture* lightmapTexture;

PbrMaterial* createPbrMaterial(ShaderProgram* shader) {
    PbrMaterial* material = malloc(sizeof(PbrMaterial));
    material->shader = shader;
    material->albedo = NULL;
    material->normal = NULL;
    material->roughness = NULL;
    material->metallic = NULL;
    material->ao = NULL;
    arrput(scene.pbrMaterials, material);
    return material;
}

void bindPbrMaterial(const PbrMaterial* material) {
    bindShaderProgram(material->shader);

    if (material->albedo != NULL) bindTexture(material->albedo, GL_TEXTURE0);
    if (material->normal != NULL) bindTexture(material->normal, GL_TEXTURE1);
    if (material->roughness != NULL) bindTexture(material->roughness, GL_TEXTURE2);
    if (material->metallic != NULL) bindTexture(material->metallic, GL_TEXTURE3);
    if (material->ao != NULL) bindTexture(material->ao, GL_TEXTURE4);

    bindTexture(scene.ibl.ggxLutTexture, GL_TEXTURE5);
    bindCubemapTexture(scene.ibl.lambertianTexture, GL_TEXTURE6);
    bindCubemapTexture(scene.ibl.ggxTexture, GL_TEXTURE7);

    //bindTexture(lightmapTexture, GL_TEXTURE8);
}

void deletePbrMaterial(PbrMaterial* material) {
    free(material);
}
