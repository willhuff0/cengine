#version 310 es

layout (location = 0) in vec3 a_pos;

layout (location = 0) out vec3 v_color;

layout (location = 0) uniform mat4 u_viewProjMat;

layout (location = 1) uniform vec3 i_positions[1500];
layout (location = 1501) uniform vec3 i_colors[1500];

void main() {
    v_color = i_colors[gl_InstanceID];

    vec3 position = i_positions[gl_InstanceID];
    gl_Position = u_viewProjMat * vec4(position + a_pos, 1);
}