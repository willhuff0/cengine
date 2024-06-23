//
// Created by wehuf on 6/23/2024.
//

#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"

typedef struct {
    int numMeshes;
    Mesh** meshes;
} Model;

bool createModel(Model** outModel, const char* dir, const char* path);
void deleteModel(Model* model);

void drawModel(Model* model);

#endif //MODEL_H