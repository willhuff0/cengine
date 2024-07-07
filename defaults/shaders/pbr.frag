#version 300 es
precision highp float;

in vec2 v_uv;
in vec2 v_lightmapUV;

uniform vec4 u_albedo;
uniform float u_roughness;
uniform float u_metallic;

uniform sampler2D u_tex_albedo;
uniform sampler2D u_tex_normal;
uniform sampler2D u_tex_roughness;
uniform sampler2D u_tex_metallic;
uniform sampler2D u_tex_ao;

uniform sampler2D u_lightmap;

layout (std140) uniform CEnginePbr {
    vec4 viewPos;
    vec4 lightDir;
    vec4 lightIntensity;
} pbr;

out vec4 o_fragColor;

void main() {
    o_fragColor = vec4(texture(u_lightmap, v_lightmapUV).rgb, 1.0f);
}