#include "engine.h"
#include "model.h"
#include "scene.h"
#include "components/debug_draw_comp.h"
#include "components/mesh_comp.h"

int main(int argc, const char* argv[])
{
    initEngine();

    createScene();

    if (!createCubemapTextureFromPaths(&scene.skybox.cubemap, DEFAULT_TEXTURE_CUBEMAP_SKYBOX)) {
        fprintf(stderr, "Failed to load skybox cubemap.\n");
        exit(49);
    }

    if (!createTextureFromPath(&lightmap, "/Users/will/CLionProjects/cengine/test/lightmap/Lightmap.tga")) {
        fprintf(stderr, "Failed to load skybox cubemap..\n");
        exit(50);
    }

    // Texture* hdriTexture;
    // if (!createHDRITextureFromPath(&hdriTexture, DEFAULT_TEXTURE_HDRI_SKYBOX)) {
    //     fprintf(stderr, "[IBL] Failed to load skybox hdri.\n");
    //     exit(49);
    // }
    // setupSceneSkybox(hdriTexture);

    //createTextureFromPath(&lightmapTexture, "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\Lightmap2.tga");

    ShaderProgram* sp;
    createShaderProgram(&sp, DEFAULT_SHADER_PBR);

    PbrModel model;
    loadPbrModel(&model, sp, "/Users/will/CLionProjects/cengine/test/lightmap", "/Users/will/CLionProjects/cengine/test/lightmap/untitled.glb");

    for (int i = 0; i < model.numMeshes; ++i) {
        Node* node = createNode();

        TransComp* transComp = addTransComp(node);

        MeshComp* meshComp = addMeshComp(node);
        meshComp->meshType = MESH_PBR;
        meshComp->mesh.pbrMesh = model.meshes[i];
    }

    //
    Node* node = createNode();

    TransComp* transComp = addTransComp(node);
    transComp->pos[2] = 10.0f;

    DebugDrawComp* debugDraw = addDebugDrawComp(node);
    debugDraw->type = DEBUG_SPHERE;
    //

    freePbrModel(&model);

    engineLoop();

    unloadScene();

    freeEngine();

    return 0;
}
