#include "engine.h"
#include "model.h"
#include "scene.h"

int main(int argc, const char* argv[])
{
    initEngine();

    createScene();

    ShaderProgram* sp;
    createShaderProgram(&sp, DEFAULT_SHADER_SIMPLE);

    SimpleModel model;
    loadSimpleModel(&model, sp, DEFAULT_MODEL_SHAPES_CUBE);

    engineLoop();

    unloadScene();

    freeEngine();

    return 0;
}
