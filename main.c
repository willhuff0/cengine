#include <stdio.h>

#include "engine.h"
#include "scene.h"
#include "shader.h"

int main(int argc, const char* argv[])
{
    initEngine();

    createScene();

    engineLoop();

    unloadScene();
    freeEngine();
    return 0;
}
