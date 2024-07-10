//
// Created by Will on 7/7/24.
//

#include "player.h"

#include "../engine.h"
#include "../game.h"
#include "../input.h"
#include "../physics.h"
#include "../visual_state.h"

#define PLAYER_CAMERA_HEIGHT 2.0f
#define PLAYER_JUMP_HEIGHT 1.5f
#define PLAYER_JUMP_VELOCITY sqrtf(-2.0f * PHYSICS_GRAVITY * PLAYER_JUMP_HEIGHT)
#define PLAYER_WALK_MOVE_SPEED 3.0f
#define PLAYER_RUN_MOVE_SPEED 6.0f
#define PLAYER_LOOK_SENSITIVITY 0.05f

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
    lPlayer->lookX = glm_clamp(lPlayer->lookX - inputGetMouseDeltaY() * PLAYER_LOOK_SENSITIVITY, -89.99f, 89.99f);
    lPlayer->lookY += inputGetMouseDeltaX() * PLAYER_LOOK_SENSITIVITY;

    vec3 playerForward;
    playerForward[0] = cosf(glm_rad(lPlayer->lookY)) * cosf(glm_rad(lPlayer->lookX));
    playerForward[1] = 0.0f;
    playerForward[2] = sinf(glm_rad(lPlayer->lookY)) * cosf(glm_rad(lPlayer->lookX));
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

    move[0] *= speed * tickArgs.deltaTime;
    move[1] *= speed * tickArgs.deltaTime;
    move[2] *= speed * tickArgs.deltaTime;

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

         lPlayer->yVelocity += PHYSICS_GRAVITY * tickArgs.deltaTime;
     }
    lPlayer->position[1] += lPlayer->yVelocity * tickArgs.deltaTime;

    //printf("POS: (%f, %f, %f), YVEL: %f\n", lPlayer->position[0], lPlayer->position[1], lPlayer->position[2], lPlayer->yVelocity);
}
void tickRemotePlayer(RemotePlayer* rPlayer) { }

// Visual

void visualizeLocalPlayer(LocalPlayer* lPlayer, VPlayer* vPlayer, VCamera* vCamera) {
    glm_vec3_copy(lPlayer->position, vPlayer->position);

    vPlayer->lookX = lPlayer->lookX;
    vPlayer->lookY = lPlayer->lookY;

    glm_vec3_add(lPlayer->position, (vec3){0.0f, PLAYER_CAMERA_HEIGHT, 0.0f}, vCamera->position);

    vCamera->forward[0] = cosf(glm_rad(lPlayer->lookY)) * cosf(glm_rad(lPlayer->lookX));
    vCamera->forward[1] = sinf(glm_rad(lPlayer->lookX));
    vCamera->forward[2] = sinf(glm_rad(lPlayer->lookY)) * cosf(glm_rad(lPlayer->lookX));
    glm_normalize(vCamera->forward);
}
void visualizeRemotePlayer(RemotePlayer* rPlayer, VPlayer* vPlayer) {
    glm_vec3_copy(rPlayer->position, vPlayer->position);

    vPlayer->lookX = rPlayer->lookX;
    vPlayer->lookY = rPlayer->lookY;
}

//