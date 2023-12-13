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

float cdot(vec3 lhs, vec3 rhs)
{
    return max(dot(lhs, rhs), 0.0);
}

void main() {
    const ivec2 coord = ivec2(gl_FragCoord.xy);
    vec3 normalFetched = texelFetch(in_normal, coord, 0).xyz;
    vec4 albedoFetched = texelFetch(in_albedo, coord, 0);
    vec3 sun_dir_norm = normalize(uSun.direction);
    out_color = albedoFetched * vec4(uSun.color * uSun.intensity * max(dot(normalFetched, -sun_dir_norm), 0.), 1.);
}
