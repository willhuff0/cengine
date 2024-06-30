#include "engine.h"
#include "model.h"
#include "scene.h"

int main(int argc, const char* argv[])
{
    initEngine();

    createScene();

    Texture* hdriTexture;
    if (!createHDRITextureFromPath(&hdriTexture, DEFAULT_TEXTURE_HDRI_SKYBOX)) {
        fprintf(stderr, "[IBL] Failed to load skybox hdri.\n");
        exit(49);
    }
    setupSceneIbl(&scene.ibl, hdriTexture);

    //createTextureFromPath(&lightmapTexture, "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\Lightmap2.tga");

    ShaderProgram* sp;
    createShaderProgram(&sp, DEFAULT_SHADER_PBR);

    PbrModel boomBox;
    loadPbrModel(&boomBox, sp, "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\Sponza-master", "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\Sponza-master\\sponza.obj");

    engineLoop();

    unloadScene();

    freeEngine();

    return 0;
}
