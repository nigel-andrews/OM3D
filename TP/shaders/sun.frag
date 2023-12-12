#version 450

layout(location = 0) out vec4 out_color;

struct SunLight
{
    vec3 direction;
    float intensity;
    vec3 color;
};

layout(binding = 0) uniform sampler2D in_albedo;
layout(binding = 1) uniform sampler2D in_normal;
layout(binding = 2) uniform sampler2D in_depth;
layout(location = 0) uniform SunLight uSun;
// layout(binding = 2) uniform sampler2D in_depth; // Is this useful ??

void main() {
    const ivec2 coord = ivec2(gl_FragCoord.xy);
    vec3 normalFetched = texelFetch(in_normal, coord, 0).xyz;
    vec4 albedoFetched = texelFetch(in_albedo, coord, 0);
    out_color = albedoFetched * dot(normalFetched, uSun.direction) * uSun.intensity;
}
