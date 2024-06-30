#version 310 es
precision highp float;

#define MATH_PI 3.1415926535897932384626433832795

layout (location = 0) in vec2 v_texCoord;
out vec4 o_fragColor;

layout (location = 0) uniform int u_currentFace;

layout (binding = 0) uniform sampler2D u_equirectangularMap;

vec3 uvToXYZ(int face, vec2 uv) {
    if (face == 0) return vec3(1.0f, uv.y, -uv.x);
    if (face == 1) return vec3(-1.0f, uv.y, uv.x);
    if (face == 2) return vec3(uv.x, -1.0f, uv.y);
    if (face == 3) return vec3(uv.x, 1.0f, -uv.y);
    if (face == 4) return vec3(uv.x, uv.y, 1.0f);
    if (face == 5) return vec3(-uv.x, uv.y, -1.0f);
}

vec2 dirToUv(vec3 dir) {
    return vec2(0.5f * 0.5f * atan(dir.z, dir.x) / MATH_PI, 1.0f - acos(dir.y) / MATH_PI);
}

vec3 equirectangularToCubeMap(int face, vec2 texCoord) {
    vec2 texCoordNew = texCoord * 2.0f - 1.0f;
    vec3 scan = uvToXYZ(face, texCoordNew);
    vec3 dir = normalize(scan);
    vec2 src = dirToUv(dir);

    return texture(u_equirectangularMap, src).rgb;
}

void main() {
    o_fragColor = vec4(equirectangularToCubeMap(u_currentFace, v_texCoord), 1.0f);
}