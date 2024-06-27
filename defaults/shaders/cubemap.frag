#version 310 es
precision highp float;

out vec4 o_fragColor;

layout (binding = 0) uniform samplerCube u_cubemap;

layout (std140, binding = 1) uniform CEnginePbr {
    vec4 viewPos;
    vec4 lightDir;
    vec4 lightIntensity;
    float envIntensity;
    float envBlurNormalized;
    float mipCount;
    float extra;
} pbr;

layout (location = 0) in vec3 v_uv;

void main() {
    vec3 color = texture(u_cubemap, v_uv).rgb; //textureLod(u_cubemap, v_uv, pbr.envBlurNormalized * float(pbr.mipCount - 1)) * pbr.envIntensity;

    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f / 2.2f));

    o_fragColor = vec4(color, 1.0f);
}