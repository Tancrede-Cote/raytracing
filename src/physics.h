#pragma once
#include "config.h"

class Sphere
{
    vec3 v = vec3(0, 0, 0);
    vec3 a = vec3(0, 0, 0);
    vec3 F = vec3(0, 0, 0);
    float m = 1.2;

public:
    vec3 _pos = vec3(0, 0, 0);
    Sphere(vec3 pos, unsigned int shader) : _pos(pos) {}
    vec3 pos() { return _pos; }
    void update(float dt);
    bool triangleCollide(vec3, vec3, vec3);
    float d(vec3, vec3);
    float r = 0.3;
    float jump = false;
};
