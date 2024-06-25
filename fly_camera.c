//
// Created by wehuf on 6/23/2024.
//

#include "fly_camera.h"

#include <GLFW/glfw3.h>

#include "engine.h"
#include "scene.h"

#define FPS_CAMERA_MOVE_SPEED 1.5f
#define FPS_CAMERA_SPRINT_MOVE_SPEED 3.0f
#define FPS_CAMERA_LOOK_SENSITIVITY 5.0f

static float lookX = 0.0f;
static float lookY = -90.0f;

void setupFlyCamera() {
    glfwSetInputMode(engine.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(engine.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void tickFlyCamera(float delta) {
    double cursorX, cursorY;
    glfwGetCursorPos(engine.window, &cursorX, &cursorY);
    glfwSetCursorPos(engine.window, 0.0, 0.0f);

    lookX = glm_clamp(lookX - cursorY * FPS_CAMERA_LOOK_SENSITIVITY * delta, -89.99f, 89.99f);
    lookY += cursorX * FPS_CAMERA_LOOK_SENSITIVITY * delta;

    scene.camera.forward[0] = cosf(glm_rad(lookY)) * cosf(glm_rad(lookX));
    scene.camera.forward[1] = sinf(glm_rad(lookX));
    scene.camera.forward[2] = sinf(glm_rad(lookY)) * cosf(glm_rad(lookX));
    glm_normalize(scene.camera.forward);

    vec3 cameraRight;
    glm_cross(scene.camera.forward, (vec3){0.0f, 1.0f, 0.0f}, cameraRight);
    glm_normalize(cameraRight);

    vec3 move = {0.0f, 0.0f, 0.0f};
    if (glfwGetKey(engine.window, GLFW_KEY_W) == GLFW_PRESS) glm_vec3_add(move, scene.camera.forward, move);
    if (glfwGetKey(engine.window, GLFW_KEY_S) == GLFW_PRESS) glm_vec3_sub(move, scene.camera.forward, move);
    if (glfwGetKey(engine.window, GLFW_KEY_D) == GLFW_PRESS) glm_vec3_add(move, cameraRight, move);
    if (glfwGetKey(engine.window, GLFW_KEY_A) == GLFW_PRESS) glm_vec3_sub(move, cameraRight, move);
    if (glfwGetKey(engine.window, GLFW_KEY_SPACE) == GLFW_PRESS) move[1] += 1.0f;
    if (glfwGetKey(engine.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) move[1] -= 1.0f;
    glm_normalize(move);

    float speed = glfwGetKey(engine.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? FPS_CAMERA_SPRINT_MOVE_SPEED : FPS_CAMERA_MOVE_SPEED;

    move[0] *= speed * delta;
    move[1] *= speed * delta;
    move[2] *= speed * delta;

    glm_vec3_add(scene.camera.position, move, scene.camera.position);
}