//
// Created by Will on 7/8/24.
//

#ifndef PACKETS_H
#define PACKETS_H

#include "common.h"

typedef struct {
    int playerId;
    vec3 position;
    vec3 linearVelocity;
    float lookX;
    float lookY;
} StcSyncPlayerPacket;

#endif //PACKETS_H
