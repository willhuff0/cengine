#include "engine.h"
#include "model.h"
#include "scene.h"

int main(int argc, const char* argv[])
{
    initEngine();

    createScene();

    // ShaderProgram* sp;
    // createShaderProgram(&sp, DEFAULT_SHADER_PBR);
    //
    // PbrModel boomBox;
    // loadPbrModel(&boomBox, sp, "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\sponza", "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\sponza\\Sponza.gltf");

    engineLoop();

    unloadScene();

    freeEngine();

    return 0;
}
