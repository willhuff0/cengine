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

    vec3 directionalLightDirection = {0.5f, -1.0f, 0.5f};
    glm_normalize(directionalLightDirection);
    glUniform3f(3, directionalLightDirection[0], directionalLightDirection[1], directionalLightDirection[2]); // DirectionalLightDirection

    if (!createModel(NULL, "C:\\Users\\wehuf\\CLionProjects\\cengine\\test", "C:\\Users\\wehuf\\CLionProjects\\cengine\\test\\environment.fbx")) {
        fprintf(stderr, "Failed to create model asset.");
        exit(89);
    }

    engineLoop();

    unloadScene();
    freeEngine();
    return 0;
}
