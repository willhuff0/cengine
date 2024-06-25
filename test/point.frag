#version 310 es
precision highp float;

out vec4 o_fragColor;

layout (location = 2) uniform vec3 u_color;

void main() {
    o_fragColor = vec4(u_color, 1.0);
}