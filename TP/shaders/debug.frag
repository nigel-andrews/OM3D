#version 450

layout(location = 0) out vec4 out_color;

layout(location = 1) in vec2 in_uv;

layout(binding = 0) uniform sampler2D in_albedo;
layout(binding = 1) uniform sampler2D in_normal;

void main() {
    const ivec2 coord = ivec2(gl_FragCoord.xy);

#ifdef DEBUG_ALBEDO
    out_color = texelFetch(in_albedo, coord, 0);
#elif defined(DEBUG_NORMAL)
    out_color = texelFetch(in_normal, coord, 0);
#endif
}
