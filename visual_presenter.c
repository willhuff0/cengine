//
// Created by Will on 7/8/24.
//

#include "visual_presenter.h"

#include "debug.h"

void initPresenter() {

}
void freePresenter() {

}

void present(const VisualState* from, const VisualState* to, const float t) {
    vec3 iPlayerPos;
    glm_vec3_lerp(from->player.position, to->player.position, t, iPlayerPos);
    debugDrawSphere(iPlayerPos, 0.5f, (vec4){1.0f, 0.0f, 0.0f, 1.0f});
}