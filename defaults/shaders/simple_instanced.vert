#version 300 es
#extension ARB_explicit_uniform_location : enable

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

layout (location = 2) in vec4 i_color;
layout (location = 3) in mat4 i_modelMat;

layout (std140, binding = 0) uniform CEngine {
    mat4 viewProjMat;
} cengine;

layout (location = 0) out vec4 v_color;

void main() {
    v_color = i_color;
    gl_Position = cengine.viewProjMat * i_modelMat * vec4(a_pos, 1.0f);
}