//
// Created by wehuf on 6/25/2024.
//

#include "simple_material.h"

#include "engine.h"
#include "scene.h"

void createSimpleMaterial(SimpleMaterial** outMaterial, ShaderProgram* shader) {
    SimpleMaterial* material = malloc(sizeof(SimpleMaterial));
    material->shader = shader;

    bindShaderProgram(shader);
    glUniformBlockBinding(shader->program, glGetUniformBlockIndex(shader->program, "CEngine"), 0);

    arrput(scene.simpleMaterials, material);
    if (outMaterial != NULL) *outMaterial = material;
}

void bindSimpleMaterial(const SimpleMaterial* material) {
    bindShaderProgram(material->shader);
}

void deleteSimpleMaterial(SimpleMaterial* material) {
    free(material);
}