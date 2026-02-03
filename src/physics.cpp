#include "config.h"

void Sphere::update(float dt) // mauvaise manière de faire, faire resting/not resting/no collision
{
    if (d(vec3(0, 0, 0), vec3(0, 1, 0)) == 0 && !(collide))
    {
        collide = true;
        float n = v.norm();
        v = vec3(0, .5, 0) * (n);
        _pos += v * (1.1f * dt);
    }
    else if (d(vec3(0, 0, 0), vec3(0, 1, 0)) == 0 && v.norm() < 0.1)
    {
        F = vec3(0);
        a = F;
        v = vec3(0);
    }
    else
    {
        collide = false;
        F = vec3(0, -9.81, 0);
        a = F;
        v += a * dt;
        _pos += v * (1.1f * dt);
    }
}

vec3 rayPlan(vec3 origin, vec3 ray, vec3 center, vec3 normal)
{
    // dot(origin+a*ray-center,normal) = 0
    // dot(origin-center,normal)+a*dot(ray,normal) = 0
    float t0 = -(origin - center).dot(normal) / ray.dot(normal);
    if (t0 > 0)
    {
        return origin + ray * t0;
    }
    return vec3(0, 0, 0);
}

float Sphere::d(vec3 origin, vec3 normal)
{ // sphere/plane distance
    vec3 temp = rayPlan(_pos, normal, origin, normal);
    if (temp.norm() == 0)
    {
        temp = rayPlan(_pos, -normal, origin, normal);
    }
    return (_pos.d(temp) > r) ? (_pos.d(temp) - r) : 0.;
}