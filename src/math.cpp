#include "math.h"

float vec4::operator[](int i)
{
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        throw std::runtime_error("invalid vec4 index");
    }
}

float vec3::operator[](int i)
{
    switch (i)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        throw std::runtime_error("invalid vec4 index");
    }
}

mat3::mat3(float x)
{
    for (int i = 0; i < 9; i++)
    {
        if (i % 4)
        {
            content[i] = 0.f;
        }
        else
        {
            content[i] = x;
        }
    }
}

mat3::mat3(vec3 a, vec3 b, vec3 c)
{ // columns
    for (int i = 0; i < 9; i++)
    {
        if (!i % 3)
        {
            content[i] = a[i / 3];
        }
        else if (i % 3 == 1)
        {
            content[i] = b[i / 3];
        }
        else if (i % 3 == 2)
        {
            content[i] = c[i / 3];
        }
    }
}

vec3 mat3::operator[](int i)
{ // returns line i
    switch (i)
    {
    case 0:
        return vec3(content[0], content[1], content[2]);
    case 1:
        return vec3(content[3], content[4], content[5]);
    case 2:
        return vec3(content[6], content[7], content[8]);
    default:
        throw std::runtime_error("wrong index for matrix");
    }
}

mat3 mat3::operator*(mat3 a)
{
    mat3 res(1.f);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                res.content[i * 3 + j] += (*this)[i][k] * a[k][j];
            }
        }
    }
    return res;
}

void mat4::print()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout << content[i * 4 + j] << " ";
        }
        std::cout << std::endl;
    }
}

mat4::mat4(float x)
{
    for (int i = 0; i < 16; i++)
    {
        if (i % 5)
        {
            content[i] = 0.f;
        }
        else
        {
            content[i] = x;
        }
    }
}

mat4::mat4(vec4 a, vec4 b, vec4 c, vec4 d)
{ // columns
    for (int i = 0; i < 16; i++)
    {
        if (!i % 4)
        {
            content[i] = a[i / 4];
        }
        else if (i % 4 == 1)
        {
            content[i] = b[i / 4];
        }
        else if (i % 4 == 2)
        {
            content[i] = c[i / 4];
        }
        else if (i % 4 == 3)
        {
            content[i] = d[i / 4];
        }
    }
}

vec4 mat4::operator[](int i)
{ // returns line i
    switch (i)
    {
    case 0:
        return vec4(content[0], content[1], content[2], content[3]);
    case 1:
        return vec4(content[4], content[5], content[6], content[7]);
    case 2:
        return vec4(content[8], content[9], content[10], content[11]);
    case 3:
        return vec4(content[12], content[13], content[14], content[15]);
    default:
        throw std::runtime_error("wrong index for matrix");
    }
}

mat4 mat4F::translation(vec3 v)
{
    vec4 a(1.f, 0.f, 0.f, 0.f);
    vec4 b(0.f, 1.f, 0.f, 0.f);
    vec4 c(0.f, 0.f, 1.f, 0.f);
    vec4 d(v);
    return mat4(a, b, c, d);
}

mat4 mat4F::diagonal(vec4 v)
{
    vec4 a(v.x, 0.f, 0.f, 0.f);
    vec4 b(0.f, v.y, 0.f, 0.f);
    vec4 c(0.f, 0.f, v.z, 0.f);
    vec4 d(0.f, 0.f, 0.f, v.w);
    return mat4(a, b, c, d);
}

mat4 mat4F::perspective(float fovy, float aspect, float n, float f)
{
    vec4 a(cosf(fovy / 2) / (sinf(fovy / 2) * aspect), 0., 0., 0.);
    vec4 b(0., cosf(fovy / 2) / sinf(fovy / 2), 0., 0.f);
    vec4 c(0., 0., (f + n) / (f - n), 1);
    vec4 d(0., 0, (2 * f * n) / (f - n), 0.);
    return mat4(a, b, c, d);
}

mat4 mat4F::lookAt(vec3 eye, vec3 at, vec3 up)
{
    vec3 zc = (eye - at).normalized();
    vec3 xc = up.cross(zc).normalized();
    vec3 yc = zc.cross(xc);
    vec4 a(xc.x, yc.x, zc.x, 0.);
    vec4 b(xc.y, yc.y, zc.y, 0.);
    vec4 c(xc.z, yc.z, zc.z, 0.);
    vec4 d(-xc.dot(eye), -yc.dot(eye), -zc.dot(eye), 1);
    return mat4(a, b, c, d);
}

mat4 mat4::operator*(mat4 a)
{
    mat4 res(1.f);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                res.content[i * 4 + j] += (*this)[i][k] * a[k][j];
            }
        }
    }
    return res;
}
