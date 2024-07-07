//
// Created by wehuf on 6/22/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

#define DEFAULT_TEXTURE_DIR DEFAULT_DIR "textures" DIR

#define DEFAULT_TEXTURE_ALBEDO_PATH    DEFAULT_TEXTURE_DIR "white.png"
#define DEFAULT_TEXTURE_NORMAL_PATH    DEFAULT_TEXTURE_DIR "normal.png"
#define DEFAULT_TEXTURE_ROUGHNESS_PATH DEFAULT_TEXTURE_DIR "white.png"
#define DEFAULT_TEXTURE_METALLIC_PATH  DEFAULT_TEXTURE_DIR "white.png"
#define DEFAULT_TEXTURE_AO_PATH        DEFAULT_TEXTURE_DIR "white.png"

#define DEFAULT_TEXTURE_HDRI_SKYBOX    DEFAULT_TEXTURE_DIR "skybox.hdr"
#define DEFAULT_TEXTURE_CUBEMAP_SKYBOX (char*[]){ \
    DEFAULT_TEXTURE_DIR "skybox" DIR "px.png", \
    DEFAULT_TEXTURE_DIR "skybox" DIR "nx.png", \
    DEFAULT_TEXTURE_DIR "skybox" DIR "py.png", \
    DEFAULT_TEXTURE_DIR "skybox" DIR "ny.png", \
    DEFAULT_TEXTURE_DIR "skybox" DIR "pz.png", \
    DEFAULT_TEXTURE_DIR "skybox" DIR "nz.png", \
}

typedef struct {
    GLuint texture;
} Texture;

typedef struct {
    char* key;
    Texture* value;
} TextureCacheEntry;

bool createTextureFromPath(Texture** outTexture, const char* path);
bool createTextureFromData(Texture** outTexture, const char* filename, uint8_t* buffer, unsigned int length);
bool createCubemapTextureFromPaths(Texture** outTexture, const char* paths[6]);
void createEmptyCubemapTexture(Texture** outTexture, int resolution, bool withMipmaps);
void createEmptyLutTexture(Texture** outTexture, int resolution);
bool createHDRITextureFromPath(Texture** outTexture, const char* path);

void deleteTexture(Texture* texture);

void bindTexture(Texture* texture, GLuint slot);
void bindCubemapTexture(Texture* texture, GLuint slot);

#endif //TEXTURE_H
