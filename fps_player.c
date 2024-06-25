//
// Created by wehuf on 6/24/2024.
//

#include "fps_player.h"

#include <GLFW/glfw3.h>

#include "engine.h"
#include "scene.h"

#define FPS_PLAYER_JUMP_HEIGHT 1.5f
#define FPS_PLAYER_GRAVITY 9.8f * 1.5f
#define FPS_PLAYER_WALK_MOVE_SPEED 3.0f
#define FPS_PLAYER_RUN_MOVE_SPEED 6.0f
#define FPS_PLAYER_LOOK_SENSITIVITY 5.0f

static vec3 position;
static float yVelocity = 0.0f;

static float lookX = 0.0f;
static float lookY = -90.0f;

static float jumpVelocity;

void setupFpsPlayer() {
    glfwSetInputMode(engine.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(engine.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glm_vec3_copy(scene.camera.position, position);
    jumpVelocity = sqrtf(2.0f * FPS_PLAYER_GRAVITY * FPS_PLAYER_JUMP_HEIGHT);
}

void tickFpsPlayer(float delta) {
    double cursorX, cursorY;
    glfwGetCursorPos(engine.window, &cursorX, &cursorY);
    glfwSetCursorPos(engine.window, 0.0, 0.0f);

    lookX = glm_clamp(lookX - cursorY * FPS_PLAYER_LOOK_SENSITIVITY * delta, -89.99f, 89.99f);
    lookY += cursorX * FPS_PLAYER_LOOK_SENSITIVITY * delta;

    scene.camera.forward[0] = cosf(glm_rad(lookY)) * cosf(glm_rad(lookX));
    scene.camera.forward[1] = sinf(glm_rad(lookX));
    scene.camera.forward[2] = sinf(glm_rad(lookY)) * cosf(glm_rad(lookX));
    glm_normalize(scene.camera.forward);

    vec3 playerForward;
    glm_vec3_copy(scene.camera.forward, playerForward);
    playerForward[1] = 0.0f;
    glm_normalize(playerForward);

    vec3 playerRight;
    glm_cross(playerForward, (vec3){0.0f, 1.0f, 0.0f}, playerRight);
    glm_normalize(playerRight);

    vec3 move = {0.0f, 0.0f, 0.0f};
    if (glfwGetKey(engine.window, GLFW_KEY_W) == GLFW_PRESS) glm_vec3_add(move, playerForward, move);
    if (glfwGetKey(engine.window, GLFW_KEY_S) == GLFW_PRESS) glm_vec3_sub(move, playerForward, move);
    if (glfwGetKey(engine.window, GLFW_KEY_D) == GLFW_PRESS) glm_vec3_add(move, playerRight, move);
    if (glfwGetKey(engine.window, GLFW_KEY_A) == GLFW_PRESS) glm_vec3_sub(move, playerRight, move);
    glm_normalize(move);

    float speed = glfwGetKey(engine.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? FPS_PLAYER_WALK_MOVE_SPEED : FPS_PLAYER_RUN_MOVE_SPEED;

    move[0] *= speed * delta;
    move[1] *= speed * delta;
    move[2] *= speed * delta;

    glm_vec3_add(position, move, position);

    if (position[1] < 0.01f) {
        // Grounded

        position[1] = 0.0f;

        if (glfwGetKey(engine.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            yVelocity = jumpVelocity;
        } else {
            yVelocity = 0.0f;
        }
    } else {
        // In air

        yVelocity -= FPS_PLAYER_GRAVITY * delta;
    }

    position[1] += yVelocity * delta;
    glm_vec3_copy(position, scene.camera.position);
}