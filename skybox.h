//
// Created by Will on 7/1/24.
//

#ifndef SKYBOX_H
#define SKYBOX_H

#include "texture.h"

typedef struct {
    Texture* cubemap;
} Skybox;

void initSkybox();
void freeSkybox();

void setupSceneSkybox(Texture* hdri);

void skyboxRenderFrame();

#endif //SKYBOX_H
