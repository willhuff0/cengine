//
// Created by wehuf on 6/23/2024.
//

#ifndef SCENE_H
#define SCENE_H

#include "mesh.h"
#include "model.h"
#include "shader.h"
#include "texture.h"

typedef struct {
    Texture* textures;
    ShaderProgram* shaders;
    Mesh* meshes;
    Model* models;
} Scene;

extern Scene scene;
extern bool sceneIsLoaded;

void createScene();
void loadScene(const char* path);
void unloadScene();

#endif //SCENE_H
