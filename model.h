//
// Created by wehuf on 6/23/2024.
//

#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

#include "pbr_mesh.h"
#include "shader.h"
#include "simple_mesh.h"

#define DEFAULT_MODEL_DIR DEFAULT_DIR "shapes" DIR

#define DEFAULT_MODEL_SHAPES_CUBE              DEFAULT_MODEL_DIR "cube.obj"
#define DEFAULT_MODEL_SHAPES_ICOSAHEDRON       DEFAULT_MODEL_DIR "icosahedron.obj"
#define DEFAULT_MODEL_SHAPES_ICOSPHERE         DEFAULT_MODEL_DIR "icosphere.obj"
#define DEFAULT_MODEL_SHAPES_ICOSPHERE_HIGHRES DEFAULT_MODEL_DIR "icosphere_highres.obj"

typedef struct {
    int numMeshes;
    SimpleMesh** meshes;
} SimpleModel;

typedef struct {
    int numMeshes;
    PbrMesh** meshes;
} PbrModel;

bool loadSimpleModel(SimpleModel* outModel, ShaderProgram* shader, const char* path);
void freeSimpleModel(SimpleModel* model);

bool loadPbrModel(PbrModel* outModel, ShaderProgram* shader, const char* dir, const char* path);
void freePbrModel(PbrModel* model);

extern PbrModel testModel;

void drawPbrModel(PbrModel* model);

#endif //MODEL_H