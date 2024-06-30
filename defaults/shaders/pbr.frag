#version 310 es
#extension GL_EXT_shader_io_blocks : enable
precision highp float;

out vec4 o_fragColor;

layout (binding = 0) uniform sampler2D u_tex_albedo;
layout (binding = 1) uniform sampler2D u_tex_normal;
layout (binding = 2) uniform sampler2D u_tex_roughness;
layout (binding = 3) uniform sampler2D u_tex_metallic;
layout (binding = 4) uniform sampler2D u_tex_ao;
layout (binding = 5) uniform sampler2D u_GGXLUT;
layout (binding = 6) uniform samplerCube u_LambertianEnvSampler;
layout (binding = 7) uniform samplerCube u_GGXEnvSampler;
layout (binding = 8) uniform sampler2D u_lightmap;

layout (std140, binding = 1) uniform CEnginePbr {
    vec4 viewPos;
    vec4 lightDir;
    vec4 lightIntensity;
} pbr;

layout (location = 0) in vec2 v_uv;
layout (location = 1) in vec2 v_lightmapUV;
layout (location = 2) in PbrTangent {
    vec3 fragPos;
    vec3 viewPos;
    vec3 lightDir;
} pbrTangent;

const float M_PI = 3.141592653589793;

const float u_EnvIntensity = 1.0f;
const int u_MipCount = 1;

float clampedDot(vec3 x, vec3 y)
{
    return clamp(dot(x, y), 0.0, 1.0);
}

vec3 getDiffuseLight(vec3 n)
{
    return texture(u_LambertianEnvSampler, n).rgb;
}

vec4 getSpecularSample(vec3 reflection, float lod)
{
    return textureLod(u_GGXEnvSampler, reflection, lod) * u_EnvIntensity;
}

//vec3 getIBLRadianceGGX(vec3 n, vec3 v, float roughness, vec3 F0, float specularWeight)
//{
//    float NdotV = clampedDot(n, v);
//    float lod = roughness * float(u_MipCount - 1);
//    vec3 reflection = normalize(reflect(-v, n));
//
//    vec2 brdfSamplePoint = clamp(vec2(NdotV, roughness), vec2(0.0, 0.0), vec2(1.0, 1.0));
//    vec2 f_ab = texture(u_GGXLUT, brdfSamplePoint).rg;
//    vec4 specularSample = getSpecularSample(reflection, lod);
//
//    vec3 specularLight = specularSample.rgb;
//
//    // see https://bruop.github.io/ibl/#single_scattering_results at Single Scattering Results
//    // Roughness dependent fresnel, from Fdez-Aguera
//    vec3 Fr = max(vec3(1.0 - roughness), F0) - F0;
//    vec3 k_S = F0 + Fr * pow(1.0 - NdotV, 5.0);
//    vec3 FssEss = k_S * f_ab.x + f_ab.y;
//
//    return specularWeight * specularLight * FssEss;
//}
//
//vec3 getIBLRadianceLambertian(vec3 n, vec3 v, float roughness, vec3 diffuseColor, vec3 F0, float specularWeight)
//{
//    float NdotV = clampedDot(n, v);
//    vec2 brdfSamplePoint = clamp(vec2(NdotV, roughness), vec2(0.0, 0.0), vec2(1.0, 1.0));
//    vec2 f_ab = texture(u_GGXLUT, brdfSamplePoint).rg;
//
//    vec3 irradiance = getDiffuseLight(n);
//
//    // see https://bruop.github.io/ibl/#single_scattering_results at Single Scattering Results
//    // Roughness dependent fresnel, from Fdez-Aguera
//
//    vec3 Fr = max(vec3(1.0 - roughness), F0) - F0;
//    vec3 k_S = F0 + Fr * pow(1.0 - NdotV, 5.0);
//    vec3 FssEss = specularWeight * k_S * f_ab.x + f_ab.y; // <--- GGX / specular light contribution (scale it down if the specularWeight is low)
//
//    // Multiple scattering, from Fdez-Aguera
//    float Ems = (1.0 - (f_ab.x + f_ab.y));
//    vec3 F_avg = specularWeight * (F0 + (1.0 - F0) / 21.0);
//    vec3 FmsEms = Ems * FssEss * F_avg / (1.0 - F_avg * Ems);
//    vec3 k_D = diffuseColor * (1.0 - FssEss + FmsEms); // we use +FmsEms as indicated by the formula in the blog post (might be a typo in the implementation)
//
//    return (FmsEms + k_D) * irradiance;
//}

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
    vec3 v = normalize(pbrTangent.viewPos - pbrTangent.fragPos);     // Vector view pos to frag pos tangent space
    vec3 n = normalize(texture(u_tex_normal, v_uv).rgb * 2.0 - 1.0); // Surface normal tangent space

    vec3 baseColor = texture(u_tex_albedo, v_uv).rgb;

    float perceptualRoughness = texture(u_tex_roughness, v_uv).g;
    float metallic = texture(u_tex_metallic, v_uv).b;

    vec3 c_diff = mix(baseColor, vec3(0.0f), metallic);
    vec3 f0 = mix(vec3(0.04f), baseColor, metallic);
    float specularWeight = 1.0f;

    float alphaRoughness = perceptualRoughness * perceptualRoughness;

    float reflectance = max(max(f0.r, f0.g), f0.b);

    vec3 f90 = vec3(1.0f);

    vec3 f_diffuse = vec3(0.0f);
    vec3 f_specular = vec3(0.0f);

    // IBL
    //f_specular += getIBLRadianceGGX(n, v, perceptualRoughness, f0, specularWeight);
    //f_diffuse += getIBLRadianceLambertian(n, v, perceptualRoughness, c_diff, f0, specularWeight);



    vec3 f_diffuse_ibl = f_diffuse;
    vec3 f_specular_ibl = f_specular;
    f_diffuse = vec3(0.0f);
    f_specular = vec3(0.0f);

    // For each light
//    vec3 l = normalize(-pbrTangent.lightDir);                        // Vector light pos to frag pos tangent space
//    vec3 h = normalize(l + v);                                       // Half vector
//
//    float NdotL = clampedDot(n, l);
//    float NdotV = clampedDot(n, v);
//    float NdotH = clampedDot(n, h);
//    float LdotH = clampedDot(l, h);
//    float VdotH = clampedDot(v, h);
//
//    if (NdotL > 0.0 || NdotV > 0.0)
//    {
//        vec3 intensity = vec3(pbr.lightIntensity);
//        vec3 l_diffuse = vec3(0.0f);
//        vec3 l_specular = vec3(0.0f);
//
//        l_diffuse += intensity * NdotL *  BRDF_lambertian(f0, f90, c_diff, specularWeight, VdotH);
//        l_specular += intensity * NdotL * BRDF_specularGGX(f0, f90, alphaRoughness, specularWeight, VdotH, NdotL, NdotV, NdotH);
//
//        f_diffuse += l_diffuse;
//        f_specular += l_specular;
//    }
    //

    vec3 diffuse;
    vec3 specular;

    //float ao = texture(u_tex_ao, v_uv).r;
    diffuse = f_diffuse + f_diffuse_ibl;
    specular = f_specular + f_specular_ibl;

    // Lightmap
    //vec3 irradiance = texture(u_lightmap, v_lightmapUV).rgb;
    //diffuse += irradiance * baseColor;
    o_fragColor = vec4(baseColor, 1.0f);

    //o_fragColor = vec4(diffuse + specular, 1.0f);
}