//
// Created by wehuf on 6/22/2024.
//

#include "engine.h"

#include <stdio.h>

#include "fly_camera.h"
#include "fps_player.h"
#include "particle.h"
#include "scene.h"

#define DEFAULT_WINDOW_WIDTH  1920/1.25
#define DEFAULT_WINDOW_HEIGHT 1080/1.25

Engine engine;

static void printEngineInfo() {
    printf("Engine version: 1.0.0\n");
    printf("GLFW version:   %s\n", glfwGetVersionString());
    printf("GL version:     %s\n", (char*)glGetString(GL_VERSION));
    printf("GL renderer:    %s\n", (char*)glGetString(GL_RENDERER));
}

static void glfw_ErrorCallback(int error, const char* description) {
    fprintf(stderr, "[ENGINE] GLFW (%d): %s\n", error, description);
}

static void glfw_FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, engine.windowWidth = width, engine.windowHeight = height);
}

static void error(const char* message) {
    fprintf(stderr, "[ENGINE]: %s\n", message);
}

void initEngine() {
    engine.window = NULL;
    engine.windowWidth = DEFAULT_WINDOW_WIDTH;
    engine.windowHeight = DEFAULT_WINDOW_HEIGHT;

    glfwSetErrorCallback(glfw_ErrorCallback);

    glfwInitHint(GLFW_ANGLE_PLATFORM_TYPE, GLFW_ANGLE_PLATFORM_TYPE_VULKAN);
    if (!glfwInit()) {
        error("Failed to initialize glfw.");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_SAMPLES, 8);
    GLFWwindow* window = glfwCreateWindow(engine.windowWidth, engine.windowHeight, "cengine", NULL, NULL);
    if (window == NULL) {
        error("Failed to create window.");
        glfwTerminate();
        return;
    }
    engine.window = window;

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(window, mode->width / 2.0f - DEFAULT_WINDOW_WIDTH / 2.0f, mode->height / 2.0f - DEFAULT_WINDOW_HEIGHT / 2.0f);

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glViewport(0, 0, engine.windowWidth, engine.windowHeight);
    glfwSetFramebufferSizeCallback(window, glfw_FramebufferResizeCallback);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    printEngineInfo();
}

FrameInfo frameInfo;

void engineLoop() {
    setupFlyCamera();
    initParticles();

    double lastFPSCounterTime = glfwGetTime();
    int nbFrames = 0;
    double lastTime = glfwGetTime();

    while(!glfwWindowShouldClose(engine.window)) {
        glfwPollEvents();

        if (glfwGetKey(engine.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(engine.window, GLFW_TRUE);

        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastFPSCounterTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
            // printf and reset timer
            printf("%d FPS\n", nbFrames);
            nbFrames = 0;
            lastFPSCounterTime += 1.0;
        }

        double time = glfwGetTime();
        double deltaTime = time - lastTime;
        lastTime = time;

        tickFlyCamera(deltaTime);
        tickParticles(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniform3f(2, scene.camera.position[0], scene.camera.position[1], scene.camera.position[2]); // WorldViewPos

        mat4 proj;
        glm_perspective(glm_rad(70.0f), (float)engine.windowWidth / (float)engine.windowHeight, 0.1f, 1000.0, proj);

        mat4 view;
        mat4 lookTarget;
        glm_vec3_add(scene.camera.position, scene.camera.forward, lookTarget);
        glm_lookat(scene.camera.position, lookTarget, (vec3){0.0f, 1.0f, 0.0f}, view);

        mat4 viewProj;
        glm_mat4_mul(proj, view, viewProj);

        frameInfo.delta = deltaTime;
        glm_mat4_copy(viewProj, frameInfo.viewProjMat);

        drawParticles();

        glfwSwapBuffers(engine.window);
    }

    freeParticles();
}

void freeEngine() {
    glfwSetErrorCallback(NULL);

    if (engine.window != NULL) {
        glfwSetWindowSizeCallback(engine.window, NULL);
        glfwDestroyWindow(engine.window);
        engine.window = NULL;
    }

    glfwTerminate();
}