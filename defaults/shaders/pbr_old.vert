#version 300 es

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec2 a_uv;

layout (std140) uniform CEngine {
    mat4 viewProjMat;
} cengine;

uniform mat4 u_modelMat;

layout (std140) uniform CEnginePbr {
    vec4 viewPos;
    vec4 lightDir;
    vec4 lightIntensity;
} pbr;

out vec2 v_uv;
out vec3 tangentFragPos;
out vec3 tangentViewPos;
out vec3 tangentLightDir;

void main() {
    vec3 fragPos = vec3(u_modelMat * vec4(a_pos, 1.0));
    v_uv = a_uv;

    mat3 normalMat = transpose(inverse(mat3(u_modelMat)));
    vec3 T = normalize(normalMat * a_tangent);
    vec3 N = normalize(normalMat * a_normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));
    tangentFragPos = TBN * fragPos;
    tangentViewPos = TBN * vec3(pbr.viewPos);
    tangentLightDir = TBN * vec3(pbr.lightDir);

    gl_Position = cengine.viewProjMat * u_modelMat * vec4(a_pos, 1.0f);
}