#version 310 es

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;

layout(location = 0) out vec2 v_texCoord;
layout(location = 1) out vec3 v_worldPos;
layout(location = 2) out vec3 v_normal;

layout(location = 0) uniform mat4 m_viewProjection;
layout(location = 1) uniform mat4 m_transform;

void main() {
    v_texCoord = a_texCoord;
    v_worldPos = vec3(vec4(a_pos, 1.0) * m_transform);
    v_normal = a_normal * mat3(m_transform);

    gl_Position = m_viewProjection * m_transform * vec4(a_pos, 1);
}