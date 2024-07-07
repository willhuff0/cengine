#version 300 es
precision highp float;

out vec4 o_fragColor;

uniform sampler2D u_equirectangularMap;

in vec3 v_localPos;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 sampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5f;
    return uv;
}

void main() {
    vec2 uv = sampleSphericalMap(normalize(v_localPos));
    vec3 color = texture(u_equirectangularMap, uv).rgb;

    o_fragColor = vec4(color, 1.0f);
}