#version 300 es
precision highp float;

out vec4 o_fragColor;

uniform samplerCube u_cubemap;

uniform int u_isHDRI; // bool

in vec3 v_uv;

void main() {
    vec3 color = texture(u_cubemap, v_uv).rgb;

    if (u_isHDRI == 1) {
        color = color / (color + vec3(1.0f));
        color = pow(color, vec3(1.0f / 2.2f));
    }

    o_fragColor = vec4(color, 1.0f);
}