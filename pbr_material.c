//
// Created by wehuf on 6/23/2024.
//

#include "pbr_material.h"

#include "engine.h"
#include "scene.h"

void createPbrMaterial(PbrMaterial** outMaterial, ShaderProgram* shader) {
    PbrMaterial* material = arraddnptr(scene.pbrMaterials, 1);
    material->shader = shader;
    if (outMaterial != NULL) *outMaterial = material;
}

void bindPbrMaterial(const PbrMaterial* material) {
    bindShaderProgram(material->shader);

    if (material->albedo != NULL) bindTexture(material->albedo, GL_TEXTURE0);
    if (material->normal != NULL) bindTexture(material->normal, GL_TEXTURE1);
    if (material->roughness != NULL) bindTexture(material->roughness, GL_TEXTURE2);
    if (material->metallic != NULL) bindTexture(material->metallic, GL_TEXTURE3);
    if (material->ao != NULL) bindTexture(material->ao, GL_TEXTURE4);
}

void deletePbrMaterial(PbrMaterial* material) {

}
