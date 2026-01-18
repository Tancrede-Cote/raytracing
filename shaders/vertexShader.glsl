#version 330 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in int colorIndex;

out vec3 fragmentColor;
out vec3 fragPos;

const vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

uniform vec3 camPos;

float d2(vec3 u, vec3 v){
    return (u.x-v.x)*(u.x-v.x)+(u.y-v.y)*(u.y-v.y)+(u.z-v.z)*(u.z-v.z);
}

void main()
{
    gl_Position = vec4(vertexPos, 1.0);
    fragPos = vertexPos;
    vec3 dotf = vec3(0.5, 0.2, 0.1);
    fragmentColor = vec3(1.,1.,1.);
}