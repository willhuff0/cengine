#version 300 es
#extension ARB_explicit_uniform_location : enable
precision highp float;

out vec4 o_fragColor;

layout (location = 0) in vec4 v_color;

void main() {
    o_fragColor = v_color;
}