#version 450

layout(location = 0) out vec4 out_color;

layout(location = 0) uniform struct SunLight
{
    vec3 direction;
    float intensity;
    vec3 color;
} uSun;
// layout(location = 1) uniform mat4 view_proj;

layout(binding = 0) uniform sampler2D in_albedo;
layout(binding = 1) uniform sampler2D in_normal;
layout(binding = 2) uniform sampler2D in_depth;

float cdot(vec3 lhs, vec3 rhs)
{
    return max(dot(lhs, rhs), 0.0);
}

// vec3 unproject(vec2 uv, float depth) {
//     mat4 inv_viewproj = inverse(view_proj);
//
//     const vec3 ndc = vec3(uv * 2.0 - vec2(1.0), depth);
//     const vec4 p = inv_viewproj * vec4(ndc, 1.0);
//
//     return p.xyz / p.w;
// }
//
void main() {
    const ivec2 coord = ivec2(gl_FragCoord.xy);
    vec3 normalFetched = texelFetch(in_normal, coord, 0).xyz;
    vec4 albedoFetched = texelFetch(in_albedo, coord, 0);
    float depthFetched = texelFetch(in_depth, coord, 0).x;
    // vec3 sun_dir_norm = normalize(uSun.direction);
    // out_color = albedoFetched * vec4(uSun.color * uSun.intensity * max(dot(normalFetched, -sun_dir_norm), 0.), 1.);
    out_color = vec4(pow(depthFetched, 0.1).xxx, 1.0);
}
