#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 256) out;

uniform mat4 projection;
uniform mat4 view;

uniform float eccentricity;
uniform float semiMajorAxis;
uniform float inclination;
uniform float longitudeOfAscendingNode;
uniform float argumentOfPeriapsis;

uniform float trueAnomalyFirst;
uniform float trueAnomalyLast;

uniform vec3 orbitalCenter;

out vec4 fColor;

#define M_PI 3.1415926535897932384626433832795

mat4 rotationAxis(float angle, vec3 axis) {
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

float ea_from_ta(float ta, float ecc) {
    return atan(sin(ta) * sqrt(1 - ecc*ecc),(cos(ta) + ecc));
}

void main()
{
    vec3 an_axis = vec3(cos(longitudeOfAscendingNode),0,sin(longitudeOfAscendingNode));
    
    for (int i = 0; i < 128; ++i) {
        float ta = trueAnomalyFirst + (trueAnomalyLast - trueAnomalyFirst) * (i / 127.0);
        float ea = ea_from_ta(ta, eccentricity);
        float longitude = ta + argumentOfPeriapsis + longitudeOfAscendingNode;
        float r = semiMajorAxis * (1 - eccentricity * cos(ea));
        
        mat4 transform = rotationAxis(inclination, an_axis)
                       * rotationAxis(longitude, vec3(0,1,0));

        gl_Position = projection * view * (vec4(orbitalCenter, 0.0) + transform * vec4(r,0,0,1));

        fColor = vec4(1.0,1.0,1.0, 1.0 - i / 128.0);
        //gl_Position = projection * view * vec4(cos(2 * M_PI * i / 127.0), 0.0, sin(2 * M_PI * i / 127.0), 1.0);

        EmitVertex();
        
    }
    EndPrimitive();
}
