#version 310 es

layout (location = 0) in vec3 a_pos;

layout (location = 0) uniform mat4 u_viewProjMat;
layout (location = 1) uniform mat4 u_modelMat;

void main() {
    gl_PointSize = 3.0f;

    gl_Position = u_viewProjMat * u_modelMat * vec4(a_pos, 1);
}