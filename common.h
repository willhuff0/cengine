//
// Created by wehuf on 6/22/2024.
//

#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

#define dSINGLE
#include <ode/ode.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLES3/gl32.h>

#include <cglm/cglm.h>

#include <stb_image.h>
#include <stb_ds.h>

#define DIR "/"
#define DEFAULT_DIR "/Users/will/CLionProjects/cengine/defaults/"

char* readFile(const char* path);

#endif //COMMON_H
