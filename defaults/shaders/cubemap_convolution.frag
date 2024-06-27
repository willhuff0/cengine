#version 310 es
precision highp float;

out vec4 o_fragColor;

layout (binding = 0) uniform samplerCube u_cubemap;

layout (location = 0) in vec3 v_localPos;

const float PI = 3.14159265359;

void main() {
    vec3 normal = normalize(v_localPos);

    vec3 irradiance = vec3(0.0f);

    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = normalize(cross(up, normal));
    up = normalize(cross(normal, right));

    float sampleDelta = 0.025f;
    float nrSamples = 0.0f;
    for (float phi = 0.0f; phi < 2.0f * PI; phi += sampleDelta) {
        for (float theta = 0.0f; theta < 0.5f * PI; theta += sampleDelta) {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            irradiance += texture(u_cubemap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0f / float(nrSamples));

    o_fragColor = vec4(irradiance, 1.0f);
}