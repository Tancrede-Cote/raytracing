#pragma once
#include <cmath>
#include <iostream>

class vec2
{
public:
    union
    {
        struct
        {
            float x;
            float y;
        };
        struct
        {
            float content[2];
        };
    };

    vec2() {}
    vec2(float x, float y) : x(x), y(y) {}
    vec2(float l) : x(l), y(l) {}
    ~vec2() {}
    float dot(vec2 u) { return x * u.x + y * u.y; }
    void normalize()
    {
        float n = sqrtf(x * x + y * y);
        x = x / n;
        y = y / n;
    }
    vec2 normalized()
    {
        float n = sqrtf(x * x + y * y);
        return vec2(x / n, y / n);
    }
    float norm()
    {
        float n = sqrtf(x * x + y * y);
        return n;
    }
    vec2 operator+(vec2 u) { return vec2(x + u.x, y + u.y); }
    vec2 operator-(vec2 u) { return vec2(x - u.x, y - u.y); }
    vec2 operator-() { return vec2(-x, -y); }
    void operator+=(vec2 u)
    {
        x += u.x;
        y += u.y;
    }
    void operator-=(vec2 u)
    {
        x -= u.x;
        y -= u.y;
    }
    vec2 operator*(float f) { return vec2(x * f, y * f); }
    float *value_ptr()
    {
        return content;
    }
    friend vec2 operator*(float, vec2);
    // friend std::ostream& operator<<(std::ostream, vec3&);
};

class vec3
{
public:
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        struct
        {
            float content[3];
        };
    };

    vec3() {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    vec3(float l) : x(l), y(l), z(l) {}
    ~vec3() {}
    float operator[](int);
    float dot(vec3 u) { return x * u.x + y * u.y + z * u.z; }
    vec3 cross(vec3 u) { return vec3(y * u.z - z * u.y, z * u.x - x * u.z, x * u.y - y * u.x); }
    vec3 copy() { return vec3(x, y, z); }
    void normalize()
    {
        float n = sqrtf(x * x + y * y + z * z);
        x = x / n;
        y = y / n;
        z = z / n;
    }
    vec3 normalized()
    {
        float n = sqrtf(x * x + y * y + z * z);
        return vec3(x / n, y / n, z / n);
    }
    float norm()
    {
        float n = sqrtf(x * x + y * y + z * z);
        return n;
    }
    vec3 operator+(vec3 u) { return vec3(x + u.x, y + u.y, z + u.z); }
    vec3 operator-(vec3 u) { return vec3(x - u.x, y - u.y, z - u.z); }
    vec3 operator-() { return vec3(-x, -y, -z); }
    void operator+=(vec3 u)
    {
        x += u.x;
        y += u.y;
        z += u.z;
    }
    void operator-=(vec3 u)
    {
        x -= u.x;
        y -= u.y;
        z -= u.z;
    }
    vec3 operator*(float f) { return vec3(x * f, y * f, z * f); }
    void operator*=(float f) { (*this) = (*this) * f; }
    vec3 operator/(float f) { return (*this) * (1 / f); }
    float d(vec3 u) { return (*this - u).norm(); } // distance between vectors viewed as points
    float *value_ptr()
    {
        return content;
    }
    void print() { std::cout << x << " " << y << " " << z; }
    friend vec3 operator*(float, vec3);
    // friend std::ostream& operator<<(std::ostream, vec3&);
};

template <typename T, typename Q>
Q operator*(T lambda, Q u)
{
    return u * lambda;
}

// std::ostream& operator<<(std::ostream& o, vec3& v){
//     o<<"("<<v.x<<","<<v.y<<","<<v.z<<")";
//     return o;
// }

class vec4
{
public:
    float x{};
    float y{};
    float z{};
    float w = 1.f;
    vec4() {}
    vec4(float x, float y, float z) : x(x), y(y), z(z) {}
    vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    vec4(float l) : x(l), y(l), z(l), w(l) {}
    ~vec4() {}
    vec4(vec3 &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = 1.f;
    }
    float operator[](int);
    float dot(vec4 u) { return x * u.x + y * u.y + z * u.z + w * u.w; }
    float norm()
    {
        float n = sqrtf(x * x + y * y + z * z + w * w);
        return n;
    }
    vec4 normalized()
    {
        float n = sqrtf(x * x + y * y + z * z + w * w);
        return vec4(x / n, y / n, z / n, w / n);
    }
    void normalize()
    {
        float n = sqrtf(x * x + y * y + z * z + w * w);
        x = x / n;
        y = y / n;
        z = z / n;
        w = w / n;
    }
};

class mat3
{
private:
    float content[9]{};

public:
    float *value_ptr() { return content; }
    vec3 operator[](int);
    mat3(float);
    mat3(vec3, vec3, vec3);
    void print();
    float *get();
    mat3 operator*(mat3 A);
    vec3 operator*(vec3 v) { return vec3(content[0] * v.x + content[1] * v.y + content[2] * v.z, content[3] * v.x + content[4] * v.y + content[5] * v.z, content[6] * v.x + content[7] * v.y + content[8] * v.z); }
};

class mat4
{
private:
    float content[16]{};

public:
    float *value_ptr() { return content; }
    vec4 operator[](int);
    mat4(float);
    mat4(vec4, vec4, vec4, vec4);
    void print();
    float *get();
    mat4 operator*(mat4 A);
};

class mat4F
{
public:
    static mat4 diagonal(vec4);
    static mat4 translation(vec3);
    // static mat4 rotation(float,vec3);
    static mat4 perspective(float, float, float, float);
    static mat4 lookAt(vec3, vec3, vec3);
};
