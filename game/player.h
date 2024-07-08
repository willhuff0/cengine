//
// Created by Will on 7/7/24.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "../common.h"
#include "../packets.h"

typedef struct {
    int id;

    //dBodyID body;
    vec3 position;
    float yVelocity;

    float lookX;
    float lookY;
} LocalPlayer;

typedef struct {
    int id;

    vec3 position;

    float lookX;
    float lookY;
} RemotePlayer;

void createLocalPlayer(LocalPlayer* lPlayer, int id);
void createRemotePlayer(RemotePlayer* rPlayer, int id);

void destroyLocalPlayer(LocalPlayer* lPlayer);
void destroyRemotePlayer(RemotePlayer* rPlayer);

void syncRemotePlayer(RemotePlayer* rPlayer, StcSyncPlayerPacket packet);

void tickLocalPlayer(LocalPlayer* lPlayer);
void tickRemotePlayer(RemotePlayer* rPlayer);

// Visual

typedef struct {
    vec3 position;
    float lookX;
    float lookY;
} VPlayer;

void visualizeLocalPlayer(LocalPlayer* lPlayer, VPlayer* vPlayer);
void visualizeRemotePlayer(RemotePlayer* rPlayer, VPlayer* vPlayer);

//

#endif //PLAYER_H
