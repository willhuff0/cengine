//
// Created by wehuf on 6/23/2024.
//

#include "material.h"

void initMaterial(Material* material) {
    material->albedo = NULL;
    material->normal = NULL;
    material->roughness = NULL;
    material->metallic = NULL;
    material->ao = NULL;
}

void bindMaterial(const Material* material) {
    if (material->albedo != NULL) bindTexture(material->albedo, GL_TEXTURE0);
    if (material->normal != NULL) bindTexture(material->normal, GL_TEXTURE1);
    if (material->roughness != NULL) bindTexture(material->roughness, GL_TEXTURE2);
    if (material->metallic != NULL) bindTexture(material->metallic, GL_TEXTURE3);
    if (material->ao != NULL) bindTexture(material->ao, GL_TEXTURE4);
}