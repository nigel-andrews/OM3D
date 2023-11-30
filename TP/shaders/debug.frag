#version 450

#define NONE 0
#define ALBEDO 1
#define NORMALS 2

layout(location = 0) out vec4 out_color;

layout(location = 1) in vec2 in_uv;

layout(location = 0) uniform uint in_debug_mode;
layout(binding = 0) uniform sampler2D in_albedo;
layout(binding = 1) uniform sampler2D in_normal;

void main() {
    const ivec2 coord = ivec2(gl_FragCoord.xy);

    switch (in_debug_mode)
    {
    case ALBEDO:
        out_color = texelFetch(in_albedo, coord, 0);
        break;
    case NORMALS:
        out_color = texelFetch(in_normal, coord, 0);
        break;
    default:
        out_color = texelFetch(in_albedo, coord, 0);
        break;
    }
}
