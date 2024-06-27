//
// Created by wehuf on 6/26/2024.
//

#ifndef IBL_H
#define IBL_H

#include "texture.h"

extern Texture* cubemapTexture;
extern Texture* irradianceTexture;
extern Texture* ggxLutTexture;

void initIbl();
void iblRenderFrame();
void freeIbl();

#endif //IBL_H
