#version 310 es
precision highp float;

out vec4 FragColor;

layout(location = 0) in vec2 v_texCoord;
layout(location = 1) in vec3 v_worldPos;
layout(location = 2) in vec3 v_normal;

layout(binding = 0) uniform sampler2D texture_albedo;
layout(binding = 1) uniform sampler2D texture_normal;
layout(binding = 2) uniform sampler2D texture_roughness;
layout(binding = 3) uniform sampler2D texture_metallic;
layout(binding = 4) uniform sampler2D texture_occlusion;

layout(location = 2) uniform vec3 worldViewPos;

layout(location = 3) uniform vec3  directionalLight_direction;
layout(location = 4) uniform vec3  directionalLight_color;
layout(location = 5) uniform float directionalLight_illuminance;

const float PI = 3.14159265359;

//------------------
//  Specular

// GGX NDF
float specular_distribution(float NoH, float roughness) {
    float a = NoH * roughness;
    float k = roughness / (1.0 - NoH * NoH + a * a);
    return k * k * (1.0 / PI);
}

// Smith GGX geometric shadowing
float specular_visibility(float NoV, float NoL, float roughness) {
    float a2 = roughness * roughness;
    float GGXV = NoL * sqrt(NoV * NoV * (1.0 - a2) + a2);
    float GGXL = NoV * sqrt(NoL * NoL * (1.0 - a2) + a2);
    return 0.5 / (GGXV + GGXL);
}

// Schlick Cook-Torrance approximation fresnel
vec3 specular_fresnel(float u, vec3 f0) {
    float f = pow(1.0 - u, 5.0);
    return f + f0 * (1.0 - f);
}

//------------------


//------------------
//  Diffuse

// Lambert diffuse
float diffuse_lambert() {
    return 1.0 / PI;
}

//------------------

void main() {
    vec3 baseColor = texture(texture_albedo, v_texCoord).rgb;

    float metallic = texture(texture_metallic, v_texCoord).r;

    float inRoughness = texture(texture_roughness, v_texCoord).r;
    float roughness = inRoughness * inRoughness;

    float occlusion = texture(texture_occlusion, v_texCoord).r;

    vec3 diffuseColor = (1.0 - metallic) * baseColor;
    vec3 f0 = 0.16 * 1.0 * 1.0 * (1.0 - metallic) + baseColor * metallic;

    vec3 n = v_normal;
    vec3 v = normalize(v_worldPos - worldViewPos);
    vec3 l = normalize(-directionalLight_direction);

    vec3 h = normalize(v + l);

    float NoV = abs(dot(n, v)) + 1e-5;
    float NoL = clamp(dot(n, l), 0.0, 1.0);
    float NoH = clamp(dot(n, h), 0.0, 1.0);
    float LoH = clamp(dot(l, h), 0.0, 1.0);

    float D = specular_distribution(NoH, roughness);
    vec3 F = specular_fresnel(LoH, f0);
    float V = specular_visibility(NoV, NoL, roughness);

    vec3 specular = (D * V) * F;
    vec3 diffuse = diffuseColor * diffuse_lambert();
    vec3 brdf = diffuse + specular;

    float illuminance = directionalLight_illuminance * NoL;
    vec3 luminance = brdf * illuminance;

    FragColor = vec4(luminance, 1.0);
}