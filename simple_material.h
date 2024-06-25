//
// Created by wehuf on 6/23/2024.
//

#ifndef MATERIAL_H
#define MATERIAL_H

typedef struct {
    ShaderProgram*
} SimpleMaterial;

void initSimpleMaterial(SimpleMaterial* material);
void bindSimpleMaterial(const SimpleMaterial* material);

#endif //MATERIAL_H
