#version 310 es

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec3 a_bitangent;
layout(location = 4) in vec2 a_uv;

layout(location = 0) out vec3 v_fragPos;
layout(location = 1) out vec2 v_uv;
layout(location = 2) out vec3 v_tangentLightDir;
layout(location = 3) out vec3 v_tangentViewPos;
layout(location = 4) out vec3 v_tangentFragPos;

layout(location = 0) uniform mat4 u_viewProjMat;
layout(location = 1) uniform mat4 u_modelMat;
layout(location = 2) uniform vec3 u_viewPos;
layout(location = 3) uniform vec3 u_lightDir;

void main() {
    v_fragPos = vec3(u_modelMat * vec4(a_pos, 1.0));
    v_uv = a_uv;

    mat3 normalMat = transpose(inverse(mat3(u_modelMat)));
    vec3 T = normalize(normalMat * a_tangent);
    vec3 N = normalize(normalMat * a_normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    v_tangentLightDir = TBN * u_lightDir;
    v_tangentViewPos = TBN * u_viewPos;
    v_tangentFragPos = TBN * v_fragPos;

    gl_Position = u_viewProjMat * u_modelMat * vec4(a_pos, 1);
}