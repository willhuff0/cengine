//
// Created by wehuf on 6/23/2024.
//

#ifndef SIMPLE_MATERIAL_H
#define SIMPLE_MATERIAL_H

#include "shader.h"

typedef struct {
    ShaderProgram* shader;
} SimpleMaterial;

void createSimpleMaterial(SimpleMaterial** outMaterial, ShaderProgram* shader);
void bindSimpleMaterial(const SimpleMaterial* material);
void deleteSimpleMaterial(SimpleMaterial* material);

#endif //SIMPLE_MATERIAL_H
