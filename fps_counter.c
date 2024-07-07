//
// Created by Will on 7/6/24.
//

#include "fps_counter.h"

#include <math.h>
#include <stdio.h>
#include <glfw/glfw3.h>

#define CAPTURE_DURATION 2.0

static double captureStartTime;
static int frames;

void startFpsCounter() {
    captureStartTime = glfwGetTime();
    frames = 0;
}
void tickFpsCounter() {
    frames++;

    double currentTime = glfwGetTime();
    if (currentTime - captureStartTime >= CAPTURE_DURATION) {
        printf("%d FPS (t=%d)\n", (int)round(frames / CAPTURE_DURATION), (int)currentTime);

        captureStartTime = currentTime;
        frames = 0;
    }
}