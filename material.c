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
    if (material->albedo != NULL) bindTexture(material->albedo, 0);
    if (material->normal != NULL) bindTexture(material->normal, 1);
    if (material->roughness != NULL) bindTexture(material->roughness, 2);
    if (material->metallic != NULL) bindTexture(material->metallic, 3);
    if (material->ao != NULL) bindTexture(material->ao, 4);
}