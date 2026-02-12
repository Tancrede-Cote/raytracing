#pragma once
#include "config.h"

class Sphere
{
    vec3 _pos = vec3(0, 0, 0);
    vec3 v = vec3(0, 0, 0);
    vec3 a = vec3(0, 0, 0);
    vec3 F = vec3(0, 0, 0);
    float r = 0.1;
    bool collide = false;
    float m = 2.;

public:
    Sphere(vec3 pos) : _pos(pos) {}
    vec3 pos() { return _pos; }
    void update(float dt);
    float d(vec3, vec3);
};

typedef struct
{
    vec3 o;
    vec3 normal;
} Plane;