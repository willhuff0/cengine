//
// Created by wehuf on 6/23/2024.
//

#include "scene.h"

Scene scene;
bool sceneIsLoaded = false;

static void initScene() {
    glm_vec3((vec3){0.0f, 0.0f, 3.0f}, scene.camera.position);
    glm_vec3((vec3){0.0f, 0.0f, -1.0f}, scene.camera.forward);
    scene.textures = NULL;
    scene.shaders = NULL;
    scene.simpleMaterials = NULL;
    scene.simpleMeshes = NULL;
    scene.pbrMaterials = NULL;
    scene.pbrMeshes = NULL;
}

// typedef enum {
//     ASSET_TEXTURE,
//     ASSET_SHADER,
//     ASSET_MATERIAL,
//     ASSET_MESH,
//     ASSET_END,
// } SceneAsset;
//
// typedef enum {
//     NODE_CAMERA,
//     NODE_MODEL,
// } SceneNode;
//
// static void loadAssets(FILE* file) {
//     while (!feof(file)) {
//         unsigned char type = fgetc(file);
//         switch(type) {
//             case ASSET_TEXTURE: {
//                 int pathLen;
//                 fread(&pathLen, sizeof(int), 1, file);
//
//                 char* path = malloc(pathLen + 1);
//                 int bytesRead = fread(&path, sizeof(char), pathLen, file);
//                 path[bytesRead] = '\0';
//
//                 createTexture(NULL, path);
//
//                 free(path);
//                 break;
//             }
//             case ASSET_SHADER: {
//                 int vertPathLen;
//                 fread(&vertPathLen, sizeof(int), 1, file);
//
//                 char* vertPath = malloc(vertPathLen + 1);
//                 int vertBytesRead =fread(&vertPath, sizeof(char), vertPathLen, file);
//                 vertPath[vertBytesRead] = '\0';
//
//                 int fragPathLen;
//                 fread(&fragPathLen, sizeof(int), 1, file);
//
//                 char* fragPath = malloc(fragPathLen + 1);
//                 int fragBytesRead = fread(&fragPath, sizeof(char), fragPathLen, file);
//                 fragPath[fragBytesRead] = '\0';
//
//                 createShaderProgram(NULL, vertPath, fragPath);
//
//                 free(vertPath);
//                 free(fragPath);
//                 break;
//             }
//             case ASSET_END: return;
//             default: fprintf(stderr, "[SCENE] Unrecognized asset type: %d\n", type);
//         }
//     }
// }
//
// static void loadNodes(FILE* file) {
//     while (!feof(file)) {
//         unsigned char type = fgetc(file);
//         switch(type) {
//             default: fprintf(stderr, "[SCENE] Unrecognized node type: %d\n", type);
//         }
//     }
// }

void createScene() {
    if (sceneIsLoaded) unloadScene();
    else initScene();

    sceneIsLoaded = true;
}

// void loadScene(const char* path) {
//     if (sceneIsLoaded) unloadScene();
//     else initScene();
//
//     FILE* file = fopen(path, "rb");
//     if (file == NULL) {
//         fprintf(stderr, "[SCENE] Failed to load scene: %s\n", path);
//         return;
//     }
//
//     loadAssets(file);
//     loadNodes(file);
//
//     sceneIsLoaded = true;
// }

void unloadScene() {
    sceneIsLoaded = false;

    for (int i = 0; i < arrlen(scene.textures); ++i) {
        deleteTexture(&scene.textures[i]);
    }
    arrfree(scene.textures);

    for (int i = 0; i < arrlen(scene.shaders); ++i) {
        deleteShaderProgram(&scene.shaders[i]);
    }
    arrfree(scene.shaders);

    for (int i = 0; i < arrlen(scene.simpleMaterials); ++i) {
        deleteSimpleMaterial(&scene.simpleMaterials[i]);
    }
    arrfree(scene.simpleMaterials);

    for (int i = 0; i < arrlen(scene.simpleMeshes); ++i) {
        deleteSimpleMesh(&scene.simpleMeshes[i]);
    }
    arrfree(scene.simpleMeshes);

    for (int i = 0; i < arrlen(scene.pbrMaterials); ++i) {
        deletePbrMaterial(&scene.pbrMaterials[i]);
    }
    arrfree(scene.pbrMaterials);

    for (int i = 0; i < arrlen(scene.pbrMeshes); ++i) {
        deletePbrMesh(&scene.pbrMeshes[i]);
    }
    arrfree(scene.pbrMeshes);

    initScene();
}
