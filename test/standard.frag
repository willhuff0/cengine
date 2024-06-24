#version 310 es

precision highp float;

out vec4 o_fragColor;

layout(location = 0) in vec3 v_fragPos;
layout(location = 1) in vec2 v_uv;
layout(location = 2) in vec3 v_tangentLightDir;
layout(location = 3) in vec3 v_tangentViewPos;
layout(location = 4) in vec3 v_tangentFragPos;

layout(binding = 0) uniform sampler2D u_tex_albedo;
layout(binding = 1) uniform sampler2D u_tex_normal;
//layout(binding = 2) uniform sampler2D u_tex_roughness;
//layout(binding = 3) uniform sampler2D u_tex_metallic;
//layout(binding = 4) uniform sampler2D u_tex_ao;

//float D_GGX(float NoH, float roughness) {
//    float a = NoH * roughness;
//    float k = roughness / (1.0 - NoH * NoH + a * a);
//    return k * k * (1.0 / PI);
//}
//
//vec3 F_Schlick(float u, vec3 f0, float f90) {
//    return f0 + (vec3(f90) - f0) * pow(1.0 - u, 5.0);
//}
//
//float Fd_Burley(float NoV, float NoL, float LoH, float roughness) {
//    float f90 = 0.5 + 2.0 * roughness * LoH * LoH;
//    float lightScatter = F_Schlick(NoL, 1.0, f90);
//    float viewScatter = F_Schlick(NoV, 1.0, f90);
//    return lightScatter * viewScatter * (1.0 / PI);
//}
//
//float V_SmithGGXCorrelated(float NoV, float NoL, float roughness) {
//    float a2 = roughness * roughness;
//    float GGXV = NoL * sqrt(NoV * NoV * (1.0 - a2) + a2);
//    float GGXL = NoV * sqrt(NoL * NoL * (1.0 - a2) + a2);
//    return 0.5 / (GGXV + GGXL);
//}
//
//void BRDF(vec3 v, vec3 l) {
//    vec3 h = normalize(v + l);
//
//    float NoV = abs(dot(n, v)) + 1e-5;
//    float NoL = clamp(dot(n, l), 0.0, 1.0);
//    float NoH = clamp(dot(n, h), 0.0, 1.0);
//    float LoH = clamp(dot(l, h), 0.0, 1.0);
//
//    // perceptually linear roughness to roughness (see parameterization)
//    float roughness = perceptualRoughness * perceptualRoughness;
//
//    float D = D_GGX(NoH, roughness);
//    vec3  F = F_Schlick(LoH, f0);
//    float V = V_SmithGGXCorrelated(NoV, NoL, roughness);
//
//    // specular BRDF
//    vec3 Fr = (D * V) * F;
//
//    // diffuse BRDF
//    vec3 Fd = diffuseColor * Fd_Lambert();
//
//    // apply lighting...
//}
//
//void directionalLight(vec3 v) {
//    vec3 l = normalize(-directionalLight_direction);
//
//}

void main() {
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(u_tex_normal, v_uv).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // get diffuse color
    vec3 color = texture(u_tex_albedo, v_uv).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(v_tangentLightDir);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(v_tangentViewPos - v_tangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    o_fragColor = vec4(ambient + diffuse + specular, 1.0);
}