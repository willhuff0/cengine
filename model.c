//
// Created by wehuf on 6/23/2024.
//

#include "model.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "common.h"
#include "scene.h"

//#define USE_EMBEDDED_TEXTURES

static SimpleMesh* addSimpleMesh(SimpleMaterial* material, struct aiMesh* aiMesh) {
    SimpleVertex* vertices = malloc(aiMesh->mNumVertices * sizeof(SimpleVertex));
    unsigned int* indices = malloc(aiMesh->mNumFaces * 3 * sizeof(unsigned int));

    for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i) {
        SimpleVertex vertex;

        vertex.position[0] = aiMesh->mVertices[i].x;
        vertex.position[1] = aiMesh->mVertices[i].y;
        vertex.position[2] = aiMesh->mVertices[i].z;

        vertex.normal[0] = aiMesh->mNormals[i].x;
        vertex.normal[1] = aiMesh->mNormals[i].y;
        vertex.normal[2] = aiMesh->mNormals[i].z;

        vertices[i] = vertex;
    }

    for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i) {
        struct aiFace face = aiMesh->mFaces[i];
        indices[i * 3 + 0] = face.mIndices[0];
        indices[i * 3 + 1] = face.mIndices[1];
        indices[i * 3 + 2] = face.mIndices[2];
    }

    SimpleMesh* mesh;
    createSimpleMesh(&mesh, material, aiMesh->mNumVertices, vertices, aiMesh->mNumFaces * 3, indices);

    free(vertices);
    free(indices);

    return mesh;
}

bool loadSimpleModel(SimpleModel* outModel, ShaderProgram* shader, const char* path) {
    const struct aiScene* aiScene = aiImportFile(path,
                                            aiProcess_Triangulate |
                                            aiProcess_PreTransformVertices |
                                            aiProcess_JoinIdenticalVertices |
                                            aiProcess_FlipUVs |
                                            aiProcess_GenNormals);
    if (aiScene == NULL) {
        fprintf(stderr, "[MESH] Failed to load assimp scene: %s\n", aiGetErrorString());
        return false;
    }
    if (aiScene->mNumMeshes < 1) {
        fprintf(stderr, "[MESH] assimp scene has no meshes: %s\n", path);
        return false;
    }

    SimpleMaterial* material;
    createSimpleMaterial(&material, shader);

    SimpleModel model;
    model.numMeshes = aiScene->mNumMeshes;
    model.meshes = malloc(aiScene->mNumMeshes * sizeof(SimpleMesh*));

    for (int i = 0; i < aiScene->mNumMeshes; ++i) {
        model.meshes[i] = addSimpleMesh(material, aiScene->mMeshes[i]);
    }

    aiReleaseImport(aiScene);

    *outModel = model;
    return true;
}

void freeSimpleModel(SimpleModel* model) {
    free(model->meshes);
}

static PbrMesh* addPbrMesh(struct aiMesh* aiMesh, PbrMaterial** materials) {
    PbrVertex* vertices = malloc(aiMesh->mNumVertices * sizeof(PbrVertex));
    unsigned int* indices = malloc(aiMesh->mNumFaces * 3 * sizeof(unsigned int));

    for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i) {
        PbrVertex vertex;

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

        vertex.lightmapUV[0] = aiMesh->mTextureCoords[1][i].x;
        vertex.lightmapUV[1] = aiMesh->mTextureCoords[1][i].y;

        vertices[i] = vertex;
    }

    for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i) {
        struct aiFace face = aiMesh->mFaces[i];
        indices[i * 3 + 0] = face.mIndices[0];
        indices[i * 3 + 1] = face.mIndices[1];
        indices[i * 3 + 2] = face.mIndices[2];
    }

    PbrMesh* mesh;
    createPbrMesh(&mesh, materials[aiMesh->mMaterialIndex], aiMesh->mNumVertices, vertices, aiMesh->mNumFaces * 3, indices);

    free(vertices);
    free(indices);

    return mesh;
}

static void addFileTexture(Texture** texture, const char* dir, const char* path) {
    int dirLen = strlen(dir);
    int pathLen = strlen(path);

    int length = dirLen + pathLen + 2;
    char* absolutePath = malloc(length);
    memcpy(absolutePath, dir, dirLen);
    absolutePath[dirLen] = DIR[0];
    memcpy(absolutePath + dirLen + 1, path, pathLen);
    absolutePath[length - 1] = '\0';

    createTextureFromPath(texture, absolutePath);

    free(absolutePath);
}

static void addTexture(const struct aiScene* aiScene, Texture** texture, enum aiTextureType type, const char* dir, const char* defaultPath, struct aiMaterial* aiMat) {
    if (aiGetMaterialTextureCount(aiMat, type) == 0) {
        createTextureFromPath(texture, defaultPath);
        return;
    }

    struct aiString path;
    if (aiGetMaterialTexture(aiMat, type, 0, &path, NULL, NULL, NULL, NULL, NULL, NULL) != aiReturn_SUCCESS) {
        createTextureFromPath(texture, defaultPath);
        return;
    }

    if (path.data[0] == '*') {
        // Embedded texture
        struct aiTexture* aiTexture = aiScene->mTextures[atoi(&path.data[1])];
        if (aiTexture->mHeight == 0) {
            // Compressed
            createTextureFromData(texture, aiTexture->mFilename.data, (uint8_t*)aiTexture->pcData, aiTexture->mWidth);
        } else {
            // Raw
            fprintf(stderr, "Uncompressed image data is not supported!\n");
            exit(69);
        }
    } else {
        // File texture
        addFileTexture(texture, dir, path.data);
    }
}

static PbrMaterial* addPbrMaterial(const struct aiScene* aiScene, ShaderProgram* shader, const char* dir, struct aiMaterial* aiMat) {
    PbrMaterial* material = createPbrMaterial(shader);

    if (aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, material->albedo) != AI_SUCCESS || true) glm_vec4_copy((vec4){1.0f, 1.0f, 1.0f, 1.0f}, material->albedo);
    if (aiGetMaterialFloatArray(aiMat, AI_MATKEY_ROUGHNESS_FACTOR, &material->roughness, 0) != AI_SUCCESS || true) material->roughness = 1.0f;
    if (aiGetMaterialFloatArray(aiMat, AI_MATKEY_METALLIC_FACTOR, &material->metallic, 0) != AI_SUCCESS || true) material->metallic = 0.0f;

    addTexture(aiScene, &material->albedoTex,    aiTextureType_BASE_COLOR,        dir, DEFAULT_TEXTURE_ALBEDO_PATH, aiMat);
    addTexture(aiScene, &material->normalTex,    aiTextureType_NORMALS,           dir, DEFAULT_TEXTURE_NORMAL_PATH, aiMat);
    addTexture(aiScene, &material->roughnessTex, aiTextureType_DIFFUSE_ROUGHNESS, dir, DEFAULT_TEXTURE_ROUGHNESS_PATH, aiMat);
    addTexture(aiScene, &material->metallicTex,  aiTextureType_METALNESS,         dir, DEFAULT_TEXTURE_METALLIC_PATH, aiMat);
    addTexture(aiScene, &material->aoTex,        aiTextureType_AMBIENT_OCCLUSION, dir, DEFAULT_TEXTURE_AO_PATH, aiMat);

    return material;
}

bool loadPbrModel(PbrModel* outModel, ShaderProgram* shader, const char* dir, const char* path) {
    const struct aiScene* aiScene = aiImportFile(path,
                                            aiProcess_Triangulate |
                                            aiProcess_PreTransformVertices |
                                            aiProcess_JoinIdenticalVertices |
                                            aiProcess_FlipUVs |
                                            aiProcess_CalcTangentSpace |
                                            aiProcess_GenNormals
#ifdef USE_EMBEDDED_TEXTURES
    | aiProcess_EmbedTextures
#endif
                                            );
    if (aiScene == NULL) {
        fprintf(stderr, "[MODEL] Failed to load assimp scene: %s\n", aiGetErrorString());
        return false;
    }
    if (aiScene->mNumMeshes < 1) {
        fprintf(stderr, "[MODEL] assimp scene has no meshes: %s\n", path);
        aiReleaseImport(aiScene);
        return false;
    }

    PbrMaterial** materials = malloc(aiScene->mNumMaterials * sizeof(PbrMaterial*));
    if (materials == NULL) {
        fprintf(stderr, "[MODEL] Failed to allocate memory for materials.\n");
        aiReleaseImport(aiScene);
        return false;
    }

    bool success = true;
    for (int i = 0; i < aiScene->mNumMaterials; ++i) {
        materials[i] = addPbrMaterial(aiScene, shader, dir, aiScene->mMaterials[i]);
        if (materials[i] == NULL) {
            fprintf(stderr, "[MODEL] Failed to add pbr material.\n");
            success = false;
            break;
        }
    }

    if (!success) {
        free(materials);
        aiReleaseImport(aiScene);
        return false;
    }

    PbrModel model;
    model.numMeshes = aiScene->mNumMeshes;
    model.meshes = malloc(aiScene->mNumMeshes * sizeof(PbrMesh*));
    for (int i = 0; i < aiScene->mNumMeshes; ++i) {
        model.meshes[i] = addPbrMesh(aiScene->mMeshes[i], materials);
    }

    free(materials);

    aiReleaseImport(aiScene);

    *outModel = model;
    return true;
}

void freePbrModel(PbrModel* model) {
    free(model->meshes);
}

PbrModel testModel;

void drawPbrModel(PbrModel* model, mat4 modelMat) {
    for (int i = 0; i < model->numMeshes; ++i) {
        PbrMesh* mesh = model->meshes[i];
        bindPbrMesh(mesh);
        setUniformMat4(mesh->material->shader, "u_modelMat", modelMat);
        drawPbrMesh(mesh);
    }
}