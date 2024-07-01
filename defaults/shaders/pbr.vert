#version 300 es
#extension ARB_explicit_uniform_location : enable
#extension GL_EXT_shader_io_blocks : enable

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec2 a_uv;
layout(location = 4) in vec2 a_lightmapUV;

layout (std140, binding = 0) uniform CEngine {
    mat4 viewProjMat;
} cengine;

layout (location = 0) uniform mat4 u_modelMat;

layout (std140, binding = 1) uniform CEnginePbr {
    vec4 viewPos;
    vec4 lightDir;
    vec4 lightIntensity;
} pbr;

layout (location = 0) out vec2 v_uv;
layout (location = 1) out vec2 v_lightmapUV;
layout (location = 2) out PbrTangent {
    vec3 fragPos;
    vec3 viewPos;
    vec3 lightDir;
} pbrTangent;

void main() {
    vec3 fragPos = vec3(u_modelMat * vec4(a_pos, 1.0));
    v_uv = a_uv;
    v_lightmapUV = a_lightmapUV;

    mat3 normalMat = transpose(inverse(mat3(u_modelMat)));
    vec3 T = normalize(normalMat * a_tangent);
    vec3 N = normalize(normalMat * a_normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));
    pbrTangent.fragPos = TBN * fragPos;
    pbrTangent.viewPos = TBN * vec3(pbr.viewPos);
    pbrTangent.lightDir = TBN * vec3(pbr.lightDir);

    gl_Position = cengine.viewProjMat * u_modelMat * vec4(a_pos, 1.0f);
}