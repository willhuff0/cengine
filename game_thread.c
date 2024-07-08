//
// Created by Will on 7/7/24.
//

#include "game_thread.h"

#include <pthread.h>

#include "game.h"
#include "visual_state.h"

static pthread_t thread;

void initGameThread() {
    pthread_create(&thread, NULL, gameThreadEntry, NULL);
}

void freeGameThread() {
    gameThreadActivateShutdownFlag();
    pthread_join(thread, NULL);
}