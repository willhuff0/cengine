cmake_minimum_required(VERSION 3.28)
project(cengine C)

set(CMAKE_C_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

add_executable(cengine main.c
        common.c
        stb_impl.c
        engine.c
        scene.c
        shader.c
        simple_mesh.c
        pbr_material.c
        texture.c
        model.c
        stbi_impl.c
        fly_camera.c
        fps_player.c
        bounds.c
        debug.c
        simple_material.h
)

target_include_directories(cengine PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/angle)
target_link_libraries(cengine PRIVATE
        ${PROJECT_SOURCE_DIR}/libs/win/libglfw3.a
        ${PROJECT_SOURCE_DIR}/libs/win/libGLESv2.lib
        ${PROJECT_SOURCE_DIR}/libs/win/libEGL.lib
        ${PROJECT_SOURCE_DIR}/libs/win/assimp-vc143-mtd.lib
        ${PROJECT_SOURCE_DIR}/libs/win/libcglm.a
        ${PROJECT_SOURCE_DIR}/libs/win/libode_singlesd.a
)
