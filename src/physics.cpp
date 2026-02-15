#include "config.h"

extern vec3 plane;

void Sphere::update(float dt) // mauvaise manière de faire, faire resting/not resting/no collision
{
    /*
    if()
    */
    dt /= 3;
    if (d(vec3(0, 0, 0), plane) == 0) // collide
    {
        vec3 newv = v - plane * (2 * v.dot(plane)) / m;
        if (v.dot(plane) < -0.) // not resting
        {
            v -= plane * (2 * v.dot(plane)) / m;
            _pos += v * (1.f * dt);
            std::cout << 0 << std::endl;
        }
        else
        { // resting
            F -= plane.normalized() * F.dot(plane) / plane.norm();
            std::cout << "gjfgjfgj" << v.norm() << std::endl;
            a = F / m;
            v += a * dt;
            // v = 0;
            _pos += v * (1.f * dt);
        }
    }
    else
    {
        collide = false;
        F = vec3(0, -9.81, 0);
        a = F / m;
        v += a * dt;
        _pos += v * (1.f * dt);
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