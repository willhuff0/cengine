//
// Created by wehuf on 6/23/2024.
//

#include "pbr_material.h"

#include "engine.h"
#include "ibl.h"
#include "scene.h"

Texture* lightmap;

PbrMaterial* createPbrMaterial(ShaderProgram* shader) {
    PbrMaterial* material = malloc(sizeof(PbrMaterial));
    material->shader = shader;

    bindShaderProgram(shader);
    glUniformBlockBinding(shader->program, glGetUniformBlockIndex(shader->program, "CEngine"), 0);
    //glUniformBlockBinding(shader->program, glGetUniformBlockIndex(shader->program, "CEnginePbr"), 1);

    // setUniformVec4(material->shader, "u_albedo", material->albedo);
    // setUniformFloat(material->shader, "u_roughness", material->roughness);
    // setUniformFloat(material->shader, "u_metallic", material->metallic);
    //
    // setUniformInt(shader, "u_tex_albedo", 0);
    // setUniformInt(shader, "u_tex_normal", 1);
    // setUniformInt(shader, "u_tex_roughness", 2);
    // setUniformInt(shader, "u_tex_metallic", 3);
    // setUniformInt(shader, "u_tex_ao", 4);

    setUniformInt(shader, "u_lightmap", 5);

    material->albedoTex = NULL;
    material->normalTex = NULL;
    material->roughnessTex = NULL;
    material->metallicTex = NULL;
    material->aoTex = NULL;

    arrput(scene.pbrMaterials, material);
    return material;
}

void bindPbrMaterial(const PbrMaterial* material) {
    bindShaderProgram(material->shader);

    // setUniformVec4(material->shader, "u_albedo", (vec4) {1.0f, 1.0f, 1.0f, 1.0f});
    // setUniformFloat(material->shader, "u_roughness", material->roughness);
    // setUniformFloat(material->shader, "u_metallic", material->metallic);

    // if (material->albedoTex != NULL) bindTexture(material->albedoTex, GL_TEXTURE0);
    // if (material->normalTex != NULL) bindTexture(material->normalTex, GL_TEXTURE1);
    // if (material->roughnessTex != NULL) bindTexture(material->roughnessTex, GL_TEXTURE2);
    // if (material->metallicTex != NULL) bindTexture(material->metallicTex, GL_TEXTURE3);
    // if (material->aoTex != NULL) bindTexture(material->aoTex, GL_TEXTURE4);

    bindTexture(lightmap, GL_TEXTURE5);
}

void deletePbrMaterial(PbrMaterial* material) {
    free(material);
}
