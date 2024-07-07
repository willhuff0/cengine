cmake_minimum_required(VERSION 3.28)
project(cengine C)

set(CMAKE_C_STANDARD 17)

#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
#
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

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
        simple_material.c
        pbr_mesh.c
        ibl.c
        skybox.c
        physics.c
        node.c
        component.c
        components/mesh_comp.c
        components/trans_comp.c
        draw_queue.c
        components/debug_draw_comp.c
        components/debug_draw_comp.h
        fps_counter.c
        fps_counter.h
)

target_include_directories(cengine PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/angle)

if(WIN32)
    target_link_libraries(cengine PRIVATE
            ${PROJECT_SOURCE_DIR}/libs/win/libglfw3.a
            ${PROJECT_SOURCE_DIR}/libs/win/libGLESv2.lib
            ${PROJECT_SOURCE_DIR}/libs/win/libEGL.lib
            ${PROJECT_SOURCE_DIR}/libs/win/assimp-vc143-mtd.lib
            ${PROJECT_SOURCE_DIR}/libs/win/libcglm.a
            ${PROJECT_SOURCE_DIR}/libs/win/libode_singlesd.a
    )
elseif(APPLE)
    target_link_libraries(cengine PRIVATE
            ${PROJECT_SOURCE_DIR}/libs/osx/libglfw.3.dylib
            ${PROJECT_SOURCE_DIR}/libs/osx/libGLESv2.dylib
            ${PROJECT_SOURCE_DIR}/libs/osx/libEGL.dylib
            ${PROJECT_SOURCE_DIR}/libs/osx/libassimp.5.4.1.dylib
            ${PROJECT_SOURCE_DIR}/libs/osx/libcglm.a
            ${PROJECT_SOURCE_DIR}/libs/osx/libode.dylib
    )
elseif(UNIX)
    message(FATAL_ERROR "Unix is not supported")
else()
    message(FATAL_ERROR "Unknown operating system")
endif()