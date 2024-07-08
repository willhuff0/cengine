//
// Created by Will on 7/7/24.
//

#ifndef INPUT_H
#define INPUT_H

#include "common.h"

void initInput();
void freeInput();

void inputSwapStates();

float inputGetMouseDeltaX();
float inputGetMouseDeltaY();
void inputGetMouseDelta(vec2 dest);

bool inputGetKeyDown(int key);
bool inputGetKeyHeld(int key);
bool inputGetKeyUp(int key);

#endif //INPUT_H
