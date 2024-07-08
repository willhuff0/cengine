//
// Created by Will on 7/8/24.
//

#ifndef VISUAL_PRESENTER_H
#define VISUAL_PRESENTER_H

#include "visual_state.h"

void initPresenter();
void freePresenter();

void present(const VisualState* from, const VisualState* to, const float t);

#endif //VISUAL_PRESENTER_H
