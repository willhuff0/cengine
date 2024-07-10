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

#ifdef _WIN32

#define DIR "\\"
#define ROOT_DIR "C:\\Users\\wehuf\\CLionProjects\\cengine\\"

#elif __APPLE__

#define DIR "/"
#define ROOT_DIR "/Users/will/CLionProjects/cengine/"

#endif

#define DEFAULT_DIR ROOT_DIR "defaults" DIR

char* readFile(const char* path);

#endif //COMMON_H
