#version 330 core
layout (location = 0) in vec3 aPos;

out VS_OUT {
    vec4 pos;
} vs_out;

void main()
{
    vs_out.pos = vec4(aPos, 1.0);
}
