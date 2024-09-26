#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
    vec4 pos;
} gs_in[];

out vec3 fColor;

uniform mat4 modelview;
uniform mat4 projection;
uniform float axis_size;

#define M_PI 3.1415926535897932384626433832795

const vec4 x_axis = vec4(1.0, 0.0, 0.0, 0.0);
const vec4 y_axis = vec4(0.0, 1.0, 0.0, 0.0);
const vec4 z_axis = vec4(0.0, 0.0, 1.0, 0.0);

void main()
{
    vec4 center_pos = gs_in[0].pos;
    fColor = vec3(1.0, 0.0, 0.0);
    gl_Position = projection * modelview * center_pos;
    EmitVertex();
    gl_Position = projection * modelview * (center_pos + x_axis * axis_size);
    EmitVertex();
    EndPrimitive();
    fColor = vec3(0.0, 1.0, 0.0);
    gl_Position = projection * modelview * center_pos;
    EmitVertex();
    gl_Position = projection * modelview * (center_pos + y_axis * axis_size);
    EmitVertex();
    EndPrimitive();
    fColor = vec3(0.0, 0.0, 1.0);
    gl_Position = projection * modelview * center_pos;
    EmitVertex();
    gl_Position = projection * modelview * (center_pos + z_axis * axis_size);
    EmitVertex();
    EndPrimitive();
    
}
