//
// Created by wehuf on 6/25/2024.
//

#include "simple_material.h"

#include "engine.h"
#include "scene.h"

void createSimpleMaterial(SimpleMaterial** outMaterial, ShaderProgram* shader) {
    SimpleMaterial* material = arraddnptr(scene.simpleMaterials, 1);
    material->shader = shader;
    if (outMaterial != NULL) *outMaterial = material;
}

void bindSimpleMaterial(const SimpleMaterial* material) {
    bindShaderProgram(material->shader);
}

void deleteSimpleMaterial(SimpleMaterial* material) {

}