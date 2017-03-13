#version 400

#include <uniforms.glsl>
#include <vertex.glsl>

void main()
{
    out_vertex.position = vec4(in_position, 1);
    out_vertex.normal = normalize(in_normal);
    out_vertex.tangent = normalize(in_tangent);
    out_vertex.binormal = normalize(cross(out_vertex.tangent, out_vertex.normal));
    out_vertex.texcoord = in_texcoord;

    gl_Position = out_vertex.position;
}