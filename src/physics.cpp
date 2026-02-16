#include "config.h"

extern vec3 plane;
extern vec3 pa;
extern vec3 b;
extern vec3 c;

void Sphere::update(float dt)
{
    /*
    if()
    */
    dt /= 3;
    vec3 normal = (b - pa).cross(c - pa).normalized();
    if (normal.z < 0)
    {
        normal = -normal;
    }
    float dp = d(vec3(0, 0, 0), plane);
    if (dp <= 0) // collide
    {
        if (dp < -0.01)
        {
            plane.print();
            std::cout << std::endl;
            _pos -= plane * (1.f * dp);
        }
        if (v.dot(plane) < -0.1) // not resting
        {
            v -= plane * (2 * v.dot(plane)) / m;
            _pos += v * (1.f * dt);
        }
        else
        { // resting
            // std::cout << "aaaaa" << std::endl;
            F -= plane.normalized() * F.dot(plane) / plane.norm();
            a = F / m; //- v * m;
            v = jump ? v + a * dt + vec3(0, 5, 0) : (v.norm() <= 0.2 ? vec3(0) : v + a * dt);
            _pos += v * (1.f * dt);
        }
    }
    // else if (triangleCollide(pa, b, c))
    // {
    //     std::cout << "???" << std::endl;
    //     if (v.dot(normal) < -0.) // not resting
    //     {
    //         v -= normal * (2 * v.dot(normal)) / m;
    //         _pos += v * (1.f * dt);
    //     }
    //     else
    //     { // resting
    //         F -= normal.normalized() * F.dot(normal) / normal.norm();
    //         a = F / m - v * m;
    //         v = v.norm() <= 0.01 ? vec3(0) : v + a * dt;
    //         _pos += v * (1.f * dt);
    //     }
    // }
    else
    {
        // std::cout << "fdhdf" << std::endl;
        F = vec3(0, -9.81, 0);
        a = F / m - v * m;
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

bool Sphere::triangleCollide(vec3 pa, vec3 b, vec3 c)
{
    vec3 normal = (b - pa).cross(c - pa).normalized();
    vec3 proj = rayPlan(_pos, normal, pa, normal);
    if (proj.norm() == 0)
    {
        proj = rayPlan(_pos, -normal, pa, normal);
    }
    if (d(proj, _pos) > r)
    {
        return false;
    }
    if (d(_pos, (pa + b) / 2) < r || d(_pos, (pa + c) / 2) < r || d(_pos, (b + c) / 2) < r)
    {
        return true;
    }
    return false;
}

float Sphere::d(vec3 origin, vec3 normal)
{ // sphere/plane distance
    vec3 temp = rayPlan(_pos, normal, origin, normal);
    if (temp.norm() == 0)
    {
        temp = rayPlan(_pos, -normal, origin, normal);
    }
    return (_pos.d(temp) - r);
}