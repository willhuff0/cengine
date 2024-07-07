#version 300 es

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec2 a_uv;
layout(location = 4) in vec2 a_lightmapUV;

layout (std140) uniform CEngine {
    mat4 viewProjMat;
} cengine;

uniform mat4 u_modelMat;

out vec2 v_uv;
out vec2 v_lightmapUV;

void main() {
    v_uv = a_uv;
    v_lightmapUV = a_lightmapUV;

    gl_Position = cengine.viewProjMat * u_modelMat * vec4(a_pos, 1.0f);
}