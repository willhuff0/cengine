#include <stdio.h>

#include "engine.h"
#include "scene.h"
#include "shader.h"

int main(int argc, const char* argv[])
{
    initEngine();

    createScene();

    ShaderProgram* shader;
    if (!createShaderProgram(&shader, "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\standard.vert", "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\standard.frag")) {
        fprintf(stderr, "Failed to create shader asset.");
        exit(88);
    }
    bindShaderProgram(shader);

    glUniform3f(3, 0.0f, 0.0f, 1.0f); // DirectionalLightDirection

    if (!createModel(NULL, "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\glTF", "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\glTF\\BoomBox.gltf")) {
        fprintf(stderr, "Failed to create model asset.");
        exit(89);
    }

    engineLoop();

    unloadScene();
    freeEngine();
    return 0;
}
