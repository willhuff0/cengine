//
// Created by wehuf on 6/23/2024.
//

#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

#include "pbr_mesh.h"
#include "shader.h"
#include "simple_mesh.h"

#define DEFAULT_MODEL_SHAPES_CUBE              "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shapes\\cube.obj"
#define DEFAULT_MODEL_SHAPES_ICOSAHEDRON       "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shapes\\icosahedron.obj"
#define DEFAULT_MODEL_SHAPES_ICOSPHERE         "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shapes\\icosphere.obj"
#define DEFAULT_MODEL_SHAPES_ICOSPHERE_HIGHRES "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\shapes\\icosphere_highres.obj"

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

#endif //MODEL_H