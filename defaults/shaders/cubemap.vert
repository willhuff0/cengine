#version 310 es

layout(location = 0) in vec3 a_pos;

layout (std140, binding = 0) uniform CEngine {
    mat4 viewProjMat;
} cengine;

layout (location = 0) out vec3 v_uv;

void main() {
    v_uv = a_pos;
    mat4 mat = cengine.viewProjMat;
    mat[3] = vec4(0.0f, 0.0f, 0.0f, 0.1f);
    vec4 pos = mat * vec4(a_pos, 1.0f);
    gl_Position = pos.xyww;
}