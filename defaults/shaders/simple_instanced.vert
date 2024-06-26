#version 310 es

layout (location = 0) in vec3 a_pos;

layout (std140, binding = 0) uniform CEngine {
    mat4 viewProjMat;
} cengine;

layout (std140, binding = 0) uniform Instance {
    mat4 modelMat;
    vec4 color;
} instances[1024];

layout (location = 0) out vec4 v_color;

void main() {
    v_color = instances[gl_InstanceID].color;
    gl_Position = cengine.viewProjMat * instances[gl_InstanceID].modelMat * vec4(a_pos, 1.0f);
}