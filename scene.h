//
// Created by wehuf on 6/23/2024.
//

#ifndef SCENE_H
#define SCENE_H

#include "common.h"
#include "texture.h"
#include "pbr_mesh.h"
#include "simple_mesh.h"
#include "simple_material.h"

typedef struct {
    vec3 position;
    vec3 forward;
} Camera;

typedef struct {
    Camera camera;
    Texture* textures;
    ShaderProgram* shaders;
    SimpleMaterial* simpleMaterials;
    SimpleMesh* simpleMeshes;
    PbrMaterial* pbrMaterials;
    PbrMesh* pbrMeshes;
} Scene;

extern Scene scene;
extern bool sceneIsLoaded;

void createScene();
//void loadScene(const char* path);
void unloadScene();

#endif //SCENE_H
