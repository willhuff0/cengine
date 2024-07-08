//
// Created by Will on 7/7/24.
//

#include "player.h"

#include "../engine.h"
#include "../game.h"
#include "../input.h"
#include "../physics.h"
#include "../scene.h"

#define PLAYER_JUMP_HEIGHT 1.5f
#define PLAYER_JUMP_VELOCITY sqrtf(2.0f * PHYSICS_GRAVITY * PLAYER_JUMP_HEIGHT)
#define PLAYER_WALK_MOVE_SPEED 3.0f
#define PLAYER_RUN_MOVE_SPEED 6.0f
#define PLAYER_LOOK_SENSITIVITY 5.0f

void createLocalPlayer(LocalPlayer* lPlayer, int id) {
    lPlayer->id = id;

    glm_vec3_zero(lPlayer->position);

    lPlayer->lookX = 0.0f;
    lPlayer->lookY = 0.0f;
}
void createRemotePlayer(RemotePlayer* rPlayer, int id) {
    rPlayer->id = id;

    glm_vec3_zero(rPlayer->position);

    rPlayer->lookX = 0.0f;
    rPlayer->lookY = 0.0f;
}

void destroyLocalPlayer(LocalPlayer* lPlayer) { }
void destroyRemotePlayer(RemotePlayer* rPlayer) { }

void syncRemotePlayer(RemotePlayer* rPlayer, StcSyncPlayerPacket packet) {
    glm_vec3_copy(packet.position, rPlayer->position);
    // velocity
    rPlayer->lookX = packet.lookX;
    rPlayer->lookY = packet.lookY;
}

void tickLocalPlayer(LocalPlayer* lPlayer) {
    lPlayer->lookX = glm_clamp(lPlayer->lookX - inputGetMouseDeltaY() * PLAYER_LOOK_SENSITIVITY * tickArgs.deltaTime, -89.99f, 89.99f);
    lPlayer->lookY += inputGetMouseDeltaX() * PLAYER_LOOK_SENSITIVITY * tickArgs.deltaTime;

    scene.camera.forward[0] = cosf(glm_rad(lPlayer->lookY)) * cosf(glm_rad(lPlayer->lookX));
    scene.camera.forward[1] = sinf(glm_rad(lPlayer->lookX));
    scene.camera.forward[2] = sinf(glm_rad(lPlayer->lookY)) * cosf(glm_rad(lPlayer->lookX));
    glm_normalize(scene.camera.forward);

    vec3 playerForward;
    glm_vec3_copy(scene.camera.forward, playerForward);
    playerForward[1] = 0.0f;
    glm_normalize(playerForward);

    vec3 playerRight;
    glm_cross(playerForward, (vec3){0.0f, 1.0f, 0.0f}, playerRight);
    glm_normalize(playerRight);

    vec3 move = {0.0f, 0.0f, 0.0f};
    if (inputGetKeyHeld(GLFW_KEY_W)) glm_vec3_add(move, playerForward, move);
    if (inputGetKeyHeld(GLFW_KEY_S)) glm_vec3_sub(move, playerForward, move);
    if (inputGetKeyHeld(GLFW_KEY_D)) glm_vec3_add(move, playerRight, move);
    if (inputGetKeyHeld(GLFW_KEY_A)) glm_vec3_sub(move, playerRight, move);
    glm_normalize(move);

    float speed = inputGetKeyHeld(GLFW_KEY_LEFT_SHIFT) ? PLAYER_WALK_MOVE_SPEED : PLAYER_RUN_MOVE_SPEED;

    move[0] *= speed * frameArgs.deltaTime;
    move[1] *= speed * frameArgs.deltaTime;
    move[2] *= speed * frameArgs.deltaTime;

    glm_vec3_add(lPlayer->position, move, lPlayer->position);

     if (lPlayer->position[1] < 0.01f) {
         // Grounded

         lPlayer->position[1] = 0.0f;

         if (glfwGetKey(engine.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
             lPlayer->yVelocity = PLAYER_JUMP_VELOCITY;
         } else {
             lPlayer->yVelocity = 0.0f;
         }
     } else {
         // In air

         lPlayer->yVelocity -= PHYSICS_GRAVITY * tickArgs.deltaTime;
     }
    lPlayer->position[1] += lPlayer->yVelocity * tickArgs.deltaTime;

    glm_vec3_copy(lPlayer->position, scene.camera.position);
}
void tickRemotePlayer(RemotePlayer* rPlayer) { }

// Visual

void visualizeLocalPlayer(LocalPlayer* lPlayer, VPlayer* vPlayer) {
    glm_vec3_copy(lPlayer->position, vPlayer->position);

    vPlayer->lookX = lPlayer->lookX;
    vPlayer->lookY = lPlayer->lookY;
}
void visualizeRemotePlayer(RemotePlayer* rPlayer, VPlayer* vPlayer) {
    glm_vec3_copy(rPlayer->position, vPlayer->position);

    vPlayer->lookX = rPlayer->lookX;
    vPlayer->lookY = rPlayer->lookY;
}

//