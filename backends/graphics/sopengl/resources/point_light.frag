#version 330

#include "lighting.glsl"

uniform point_light light;

uniform sampler2D in_position;
uniform sampler2D in_diffuse;
uniform sampler2D in_normal;
uniform sampler2D in_texture_coordinate;

uniform mat4 view_matrix;

in vec4 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec2 vertex_texcoord;
in vec3 vertex_binormal;

out vec4 output;

void main()
{
    
    vec2 uv = calculate_texture_coordinate();
    vec3 surface_position = texture(in_position, uv).xyz;
    vec3 surface_normal = texture(in_normal, uv).xyz;

    vec3 L = surface_position - light.position;
    float light_distance = length(L);
    L = normalize(L);

    float att = attenuation(light.radius, light.falloff, light_distance);

    vec3 V = (view_matrix*vec4(0,0,-1,0)).rgb;
    vec3 o = light.intensity * light.color * orenNayarDiffuse(L,V,surface_normal,.2,.8) * att;

    output = vec4(o, 1);
}
