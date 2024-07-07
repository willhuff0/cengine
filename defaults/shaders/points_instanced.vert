#version 300 es

layout (location = 0) in vec3 i_pos;
layout (location = 1) in float i_size;
layout (location = 2) in vec4 i_color;

layout (std140) uniform CEngine {
    mat4 viewProjMat;
} cengine;

out vec4 v_color;

void main() {
    gl_Position = cengine.viewProjMat * vec4(i_pos, 1.0f);
    gl_PointSize = i_size;
    v_color = i_color;
}