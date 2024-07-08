//
// Created by wehuf on 6/22/2024.
//

#include "engine.h"

#include <stdio.h>

#include "debug.h"
#include "fly_camera.h"
#include "fps_counter.h"
#include "game.h"
#include "ibl.h"
#include "model.h"
#include "physics.h"
#include "scene.h"
#include "visual_presenter.h"

#define DEFAULT_WINDOW_WIDTH  1440/1.25
#define DEFAULT_WINDOW_HEIGHT 900/1.25

Engine engine;
FrameArgs frameArgs;

static void printEngineInfo() {
    printf("GLFW version:   %s\n", glfwGetVersionString());
    printf("GL version:     %s\n", (char*)glGetString(GL_VERSION));
    printf("GL renderer:    %s\n", (char*)glGetString(GL_RENDERER));
}

static void glfw_ErrorCallback(int error, const char* description) {
    fprintf(stderr, "[ENGINE] GLFW (%d): %s\n", error, description);
}

static void glfw_FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
    printf("%d, %d\n", width, height);
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

    glfwInitHint(GLFW_ANGLE_PLATFORM_TYPE, GLFW_ANGLE_PLATFORM_TYPE_METAL);
    if (!glfwInit()) {
        error("Failed to initialize glfw.");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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

    // int numExtensions;
    // glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    // for (int i = 0; i < numExtensions; ++i) {
    //     printf("Extension: %s\n", (char*)glGetStringi(GL_EXTENSIONS, i));
    // }

    glfwSwapInterval(1);

    glfwGetFramebufferSize(window, &engine.windowWidth, &engine.windowHeight);
    glViewport(0, 0, engine.windowWidth, engine.windowHeight);
    glfwSetFramebufferSizeCallback(window, glfw_FramebufferResizeCallback);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    //glClearColor(0.0f, 0.3f, 0.4f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glGenBuffers(1, &engine.cengineUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, engine.cengineUbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4), NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, engine.cengineUbo);

    glGenBuffers(1, &engine.cenginePbrUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, engine.cenginePbrUbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CEnginePbr), NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, engine.cenginePbrUbo);

    initSkybox();
    initPhysics();

    initVisualStates();
    initPresenter();

    printEngineInfo();
}

static void makeViewProjMat(mat4 viewProjMat) {
    mat4 proj;
    glm_perspective(glm_rad(70.0f), (float)engine.windowWidth / (float)engine.windowHeight, 0.1f, 300.0, proj);

    mat4 lookTarget;
    glm_vec3_add(scene.camera.position, scene.camera.forward, lookTarget);
    mat4 view;
    glm_lookat(scene.camera.position, lookTarget, (vec3){0.0f, 1.0f, 0.0f}, view);

    glm_mat4_mul(proj, view, viewProjMat);
}

static void renderFrame() {
    // initDrawQueue(&frameArgs.queue);
    //
    // // Draw
    //
    // executeDrawQueue(&frameArgs.queue);
    // freeDrawQueue(&frameArgs.queue);

    drawPbrModel(&testModel);

    const VisualState* previous = getPreviousVisualState();
    const VisualState* current = getCurrentVisualState();
    const float t = (glfwGetTime() - current->timestamp) / GAME_TIMESTEP;
    present(previous, current, t);

    debugRenderFrame();
    skyboxRenderFrame();
}

void engineLoop() {
    initDebug();

    setupFlyCamera();

    double lastTime = glfwGetTime();
    startFpsCounter();
    while(!glfwWindowShouldClose(engine.window)) {
        glfwPollEvents();

        if (glfwGetKey(engine.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(engine.window, GLFW_TRUE);
        if (glfwGetKey(engine.window, GLFW_KEY_P) == GLFW_PRESS) glfwSetInputMode(engine.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        double time = glfwGetTime();
        double deltaTime = time - lastTime;
        lastTime = time;
        tickFpsCounter();

        frameArgs = (FrameArgs) {time, deltaTime};
        makeViewProjMat(frameArgs.viewProjMat);
        glm_vec4(scene.camera.position, 0.0f, frameArgs.pbr.viewPos);
        glm_vec4(scene.light.dir, 0.0f, frameArgs.pbr.lightDir);
        glm_vec4(scene.light.intensity, 0.0f, frameArgs.pbr.lightIntensity);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindBuffer(GL_UNIFORM_BUFFER, engine.cengineUbo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), frameArgs.viewProjMat);

        glBindBuffer(GL_UNIFORM_BUFFER, engine.cenginePbrUbo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CEnginePbr), &frameArgs.pbr);

        renderFrame();

        glfwSwapBuffers(engine.window);
    }

    freeDebug();
}

void freeEngine() {
    freePresenter();
    freeVisualStates();

    freeSkybox();
    freePhysics();

    glDeleteBuffers(1, &engine.cengineUbo);
    glDeleteBuffers(1, &engine.cenginePbrUbo);

    glfwSetErrorCallback(NULL);

    if (engine.window != NULL) {
        glfwSetWindowSizeCallback(engine.window, NULL);
        glfwDestroyWindow(engine.window);
        engine.window = NULL;
    }

    glfwTerminate();
}