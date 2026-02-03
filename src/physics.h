#pragma once
#include "config.h"

class Sphere
{
    vec3 _pos = vec3(0, 0.5, 0);
    vec3 v;
    vec3 a;
    vec3 F = vec3(0, 0, 0);

public:
    Sphere() {}
    vec3 pos() { return _pos; }
    void update(float dt);
};