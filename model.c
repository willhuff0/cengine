//
// Created by wehuf on 6/23/2024.
//

#include "model.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "common.h"
#include "mesh.h"
#include "scene.h"

static void addTexture(Texture** texture, const char* dir, const char* path) {
    int dirLen = strlen(dir);
    int pathLen = strlen(path);

    int length = dirLen + pathLen + 2;
    char* absolutePath = malloc(length);
    memcpy(absolutePath, dir, dirLen);
    absolutePath[dirLen] = '\\';
    memcpy(absolutePath + dirLen + 1, path, pathLen);
    absolutePath[length - 1] = '\0';

    createTexture(texture, absolutePath);

    free(absolutePath);
}

static Material addMaterial(const char* dir, struct aiMaterial* aiMat) {
    Material material;
    initMaterial(&material);

    struct aiString path;
    if (aiGetMaterialTexture(aiMat, aiTextureType_BASE_COLOR,           0, &path, NULL, NULL, NULL, NULL, NULL, NULL) == aiReturn_SUCCESS) addTexture(&material.albedo, dir, path.data);
    if (aiGetMaterialTexture(aiMat, aiTextureType_NORMALS,           0, &path, NULL, NULL, NULL, NULL, NULL, NULL) == aiReturn_SUCCESS) addTexture(&material.normal, dir, path.data);
    if (aiGetMaterialTexture(aiMat, aiTextureType_DIFFUSE_ROUGHNESS, 0, &path, NULL, NULL, NULL, NULL, NULL, NULL) == aiReturn_SUCCESS) addTexture(&material.roughness, dir, path.data);
    if (aiGetMaterialTexture(aiMat, aiTextureType_METALNESS,         0, &path, NULL, NULL, NULL, NULL, NULL, NULL) == aiReturn_SUCCESS) addTexture(&material.metallic, dir, path.data);
    if (aiGetMaterialTexture(aiMat, aiTextureType_AMBIENT_OCCLUSION, 0, &path, NULL, NULL, NULL, NULL, NULL, NULL) == aiReturn_SUCCESS) addTexture(&material.ao, dir, path.data);

    return material;
}

static Mesh* addMesh(const char* dir, const struct aiScene* aiScene, struct aiMesh* aiMesh) {
    Material material = addMaterial(dir, aiScene->mMaterials[aiMesh->mMaterialIndex]);

    Vertex* vertices = malloc(aiMesh->mNumVertices * sizeof(Vertex));
    unsigned int* indices = malloc(aiMesh->mNumFaces * 3 * sizeof(unsigned int));

    for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i) {
        Vertex vertex;

        vertex.position[0] = aiMesh->mVertices[i].x;
        vertex.position[1] = aiMesh->mVertices[i].y;
        vertex.position[2] = aiMesh->mVertices[i].z;

        vertex.normal[0] = aiMesh->mNormals[i].x;
        vertex.normal[1] = aiMesh->mNormals[i].y;
        vertex.normal[2] = aiMesh->mNormals[i].z;

        vertex.tangent[0] = aiMesh->mTangents[i].x;
        vertex.tangent[1] = aiMesh->mTangents[i].y;
        vertex.tangent[2] = aiMesh->mTangents[i].z;

        vertex.tangent[0] = aiMesh->mBitangents[i].x;
        vertex.tangent[1] = aiMesh->mBitangents[i].y;
        vertex.tangent[2] = aiMesh->mBitangents[i].z;

        vertex.uv[0] = aiMesh->mTextureCoords[0][i].x;
        vertex.uv[1] = aiMesh->mTextureCoords[0][i].y;

        vertices[i] = vertex;
    }

    for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i) {
        struct aiFace face = aiMesh->mFaces[i];
        indices[i * 3 + 0] = face.mIndices[0];
        indices[i * 3 + 1] = face.mIndices[1];
        indices[i * 3 + 2] = face.mIndices[2];
    }

    Mesh* mesh;
    createMesh(&mesh, material, aiMesh->mNumVertices, vertices, aiMesh->mNumFaces * 3, indices);

    free(vertices);
    free(indices);

    return mesh;
}

bool createModel(Model** outModel, const char* dir, const char* path) {
    const struct aiScene* aiScene = aiImportFile(path,
                                            aiProcess_Triangulate |
                                            aiProcess_PreTransformVertices |
                                            aiProcess_JoinIdenticalVertices |
                                            aiProcess_FlipUVs |
                                            aiProcess_CalcTangentSpace |
                                            aiProcess_GenNormals);
    if (aiScene == NULL) {
        fprintf(stderr, "[MESH] Failed to load assimp scene: %s\n", aiGetErrorString());
        return false;
    }
    if (aiScene->mNumMeshes < 1) {
        fprintf(stderr, "[MESH] assimp scene has no meshes: %s\n", path);
        return false;
    }

    Model* model = arraddnptr(scene.models, 1);
    model->numMeshes = aiScene->mNumMeshes;
    model->meshes = malloc(aiScene->mNumMeshes * sizeof(Mesh*));

    for (int i = 0; i < aiScene->mNumMeshes; ++i) {
        model->meshes[i] = addMesh(dir, aiScene, aiScene->mMeshes[i]);
    }

    aiReleaseImport(aiScene);

    if (outModel != NULL) *outModel = model;
    return true;
}

void deleteModel(Model* model) {
    free(model->meshes);
}

void drawModel(Model* model) {
    for (int i = 0; i < model->numMeshes; ++i) {
        drawMesh(model->meshes[i]);
    }
}