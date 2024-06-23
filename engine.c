//
// Created by wehuf on 6/22/2024.
//

#include "engine.h"

#include <stdio.h>

#include "scene.h"

// #include "model.h"
// #include "scene.h"

#define DEFAULT_WINDOW_WIDTH  1080
#define DEFAULT_WINDOW_HEIGHT 768

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

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glViewport(0, 0, engine.windowWidth, engine.windowHeight);
    glfwSetFramebufferSizeCallback(window, glfw_FramebufferResizeCallback);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    printEngineInfo();
}

void engineLoop() {
    while(!glfwWindowShouldClose(engine.window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniform3f(2, 0.0f, 3.5f, 12.0f); // WorldViewPos

        mat4 proj;
        glm_perspective(glm_rad(45.0f), (float)engine.windowWidth / (float)engine.windowHeight, 0.1, 100.0, proj);
        mat4 view;
        glm_mat4_identity(view);
        glm_translate(view, (vec3){0.0f, -3.5f, -12.0f});

        mat4 viewProj;
        glm_mat4_mul(proj, view, viewProj);

        glUniformMatrix4fv(0, 1, false, viewProj);

        mat4 trans;
        glm_mat4_identity(trans);
        glm_rotate(trans, glm_rad(70.0f * glfwGetTime()), (vec3){0.0f, 1.0f, 0.0f});

        glUniformMatrix4fv(1, 1, false, trans);

        if (sceneIsLoaded) drawModel(scene.models);

        glfwSwapBuffers(engine.window);
    }
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