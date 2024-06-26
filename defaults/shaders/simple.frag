#version 310 es
precision highp float;

out vec4 o_fragColor;

layout (location = 1) uniform vec4 u_color;

void main() {
    o_fragColor = u_color;
}