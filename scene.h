//
// Created by wehuf on 6/23/2024.
//

#ifndef SCENE_H
#define SCENE_H

#include "common.h"
#include "ibl.h"
#include "texture.h"
#include "pbr_mesh.h"
#include "simple_mesh.h"
#include "simple_material.h"
#include "skybox.h"

typedef struct {
    vec3 dir;
    vec3 intensity;
} Light;

typedef struct {
    Light light;

    Skybox skybox;

    Texture** textures;
    ShaderProgram** shaders;
    SimpleMaterial** simpleMaterials;
    SimpleMesh** simpleMeshes;
    PbrMaterial** pbrMaterials;
    PbrMesh** pbrMeshes;

    TextureCacheEntry* textureCache;
} Scene;

extern Scene scene;
extern bool sceneIsLoaded;

void createScene();
//void loadScene(const char* path);
void unloadScene();

#endif //SCENE_H
