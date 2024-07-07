#version 300 es
precision highp float;

out vec4 o_fragColor;

uniform sampler2D u_tex_albedo;
uniform sampler2D u_tex_normal;
uniform sampler2D u_tex_roughness;
uniform sampler2D u_tex_metallic;
uniform sampler2D u_tex_ao;

uniform vec4 u_albedo;
uniform float u_roughness;
uniform float u_metallic;

layout (std140) uniform CEnginePbr {
    vec4 viewPos;
    vec4 lightDir;
    vec4 lightIntensity;
} pbr;

in vec2 v_uv;
in vec3 tangentFragPos;
in vec3 tangentViewPos;
in vec3 tangentLightDir;

const float M_PI = 3.141592653589793;

float clampedDot(vec3 x, vec3 y)
{
    return clamp(dot(x, y), 0.0, 1.0);
}

vec3 F_Schlick(vec3 f0, vec3 f90, float VdotH)
{
    return f0 + (f90 - f0) * pow(clamp(1.0 - VdotH, 0.0, 1.0), 5.0);
}

vec3 BRDF_lambertian(vec3 f0, vec3 f90, vec3 diffuseColor, float specularWeight, float VdotH) {
    return (1.0 - specularWeight * F_Schlick(f0, f90, VdotH)) * (diffuseColor / M_PI);
}

float V_GGX(float NdotL, float NdotV, float alphaRoughness)
{
    float alphaRoughnessSq = alphaRoughness * alphaRoughness;

    float GGXV = NdotL * sqrt(NdotV * NdotV * (1.0 - alphaRoughnessSq) + alphaRoughnessSq);
    float GGXL = NdotV * sqrt(NdotL * NdotL * (1.0 - alphaRoughnessSq) + alphaRoughnessSq);

    float GGX = GGXV + GGXL;
    if (GGX > 0.0)
    {
        return 0.5 / GGX;
    }
    return 0.0;
}

float D_GGX(float NdotH, float alphaRoughness)
{
    float alphaRoughnessSq = alphaRoughness * alphaRoughness;
    float f = (NdotH * NdotH) * (alphaRoughnessSq - 1.0) + 1.0;
    return alphaRoughnessSq / (M_PI * f * f);
}

vec3 BRDF_specularGGX(vec3 f0, vec3 f90, float alphaRoughness, float specularWeight, float VdotH, float NdotL, float NdotV, float NdotH)
{
    vec3 F = F_Schlick(f0, f90, VdotH);
    float Vis = V_GGX(NdotL, NdotV, alphaRoughness);
    float D = D_GGX(NdotH, alphaRoughness);

    return specularWeight * F * Vis * D;
}

void main() {
    vec3 v = normalize(tangentViewPos - tangentFragPos);     // Vector view pos to frag pos tangent space
    vec3 n = normalize(texture(u_tex_normal, v_uv).rgb * 2.0 - 1.0); // Surface normal tangent space

    vec3 baseColor = texture(u_tex_albedo, v_uv).rgb * u_albedo.rgb;

    float perceptualRoughness = texture(u_tex_roughness, v_uv).g * u_roughness;
    float metallic = texture(u_tex_metallic, v_uv).b * u_metallic;

    vec3 c_diff = mix(baseColor, vec3(0.0f), metallic);
    vec3 f0 = mix(vec3(0.04f), baseColor, metallic);
    float specularWeight = 1.0f;

    float alphaRoughness = perceptualRoughness * perceptualRoughness;

    vec3 f90 = vec3(1.0f);

    vec3 f_diffuse = vec3(0.0f);
    vec3 f_specular = vec3(0.0f);

    // For each light
    vec3 l = normalize(-tangentLightDir);                        // Vector light pos to frag pos tangent space
    vec3 h = normalize(l + v);                                       // Half vector

    float NdotL = clampedDot(n, l);
    float NdotV = clampedDot(n, v);
    float NdotH = clampedDot(n, h);
    float LdotH = clampedDot(l, h);
    float VdotH = clampedDot(v, h);

    if (NdotL > 0.0 || NdotV > 0.0)
    {
        vec3 intensity = vec3(pbr.lightIntensity);
        vec3 l_diffuse = vec3(0.0f);
        vec3 l_specular = vec3(0.0f);

        l_diffuse += intensity * NdotL *  BRDF_lambertian(f0, f90, c_diff, specularWeight, VdotH);
        l_specular += intensity * NdotL * BRDF_specularGGX(f0, f90, alphaRoughness, specularWeight, VdotH, NdotL, NdotV, NdotH);

        f_diffuse += l_diffuse;
        f_specular += l_specular;
    }

    o_fragColor = vec4(baseColor, 1.0f);
}