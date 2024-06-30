#version 310 es
precision highp float;

layout(location = 0) out vec2 v_texCoord;

void main(void)
{
    float x = float((gl_VertexID & 1) << 2);
    float y = float((gl_VertexID & 2) << 1);
    v_texCoord.x = x * 0.5;
    v_texCoord.y = y * 0.5;
    gl_Position = vec4(x - 1.0, y - 1.0, 0, 1);
}