#version 300 es

layout (location = 0) in vec3 a_pos;

uniform mat4 u_projMat;
uniform mat4 u_viewMat;

out vec3 v_localPos;

void main() {
    v_localPos = a_pos;
    gl_Position = u_projMat * u_viewMat * vec4(a_pos, 1.0f);
}