#version 330 core

in vec3 fragmentColor;
in vec3 fragPos;

out vec4 screenColor;

uniform vec3 camPos;

const float w = 1280.;
const float h = 720.;

float d2(vec3 u, vec3 v){
    return (u.x-v.x)*(u.x-v.x)+(u.y-v.y)*(u.y-v.y)*(h/w)*(h/w)+(u.z-v.z)*(u.z-v.z);
}

int intersect(){
    return 0;
}

void main()
{
    vec3 dotf = vec3(0.5, 0.2, 0.6);
    screenColor = vec4((d2(fragPos,dotf)<0.5)?vec3(1.0,1.0,1.0):vec3(0.0,0.0,0.0), 1.0);
}