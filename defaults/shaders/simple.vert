#version 310 es

layout (location = 0) in vec3 a_pos;

layout (std140, binding = 0) uniform CEngine {
    mat4 viewProjMat;
} cengine;

layout (location = 0) uniform mat4 u_modelMat;

void main() {
    gl_Position = cengine.viewProjMat * u_modelMat * vec4(a_pos, 1.0f);
}