//
// Created by wehuf on 6/22/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

#define DEFAULT_TEXTURE_DIR "C:\\Users\\wehuf\\CLionProjects\\cengine\\defaults\\textures\\"

#define DEFAULT_TEXTURE_ALBEDO_PATH    DEFAULT_TEXTURE_DIR "albedo_ao.png"
#define DEFAULT_TEXTURE_NORMAL_PATH    DEFAULT_TEXTURE_DIR "normal.png"
#define DEFAULT_TEXTURE_ROUGHNESS_PATH DEFAULT_TEXTURE_DIR "roughness.png"
#define DEFAULT_TEXTURE_METALLIC_PATH  DEFAULT_TEXTURE_DIR "metallic.png"
#define DEFAULT_TEXTURE_AO_PATH        DEFAULT_TEXTURE_DIR "albedo_ao.png"

#define DEFAULT_TEXTURE_HDRI_SKYBOX DEFAULT_TEXTURE_DIR "skybox2.hdr"
#define DEFAULT_TEXTURE_CUBEMAP_SKYBOX (char*[]){ \
    DEFAULT_TEXTURE_DIR "skybox\\px.png", \
    DEFAULT_TEXTURE_DIR "skybox\\nx.png", \
    DEFAULT_TEXTURE_DIR "skybox\\py.png", \
    DEFAULT_TEXTURE_DIR "skybox\\ny.png", \
    DEFAULT_TEXTURE_DIR "skybox\\pz.png", \
    DEFAULT_TEXTURE_DIR "skybox\\nz.png", \
}
#define DEFAULT_TEXTURE_GGXLUT         DEFAULT_TEXTURE_DIR "lut_ggx.png"

typedef struct {
    GLuint texture;
} Texture;

bool createTextureFromPath(Texture** outTexture, const char* path);
bool createTextureFromData(Texture** outTexture, const char* filename, uint8_t* buffer, unsigned int length);
bool createCubemapTextureFromPaths(Texture** outTexture, const char* paths[6]);
void createEmptyCubemapTexture(Texture** outTexture, int resolution);
bool createHDRITextureFromPath(Texture** outTexture, const char* path);

void deleteTexture(Texture* texture);

void bindTexture(Texture* texture, GLuint slot);

#endif //TEXTURE_H
