#pragma once
#include <cmath>
#include <iostream>

class vec3 {
public:
    float x{};
    float y{};
    float z{};
    float content[3]{};
    vec3(){}
    vec3(float x, float y, float z) : x(x), y(y), z(z){}
    ~vec3(){}
    float dot(vec3 u){return x*u.x+y*u.y+z*u.z;}
    vec3 cross(vec3 u){return vec3(y*u.z-z*u.y, z*u.x-x*u.z,x*u.y-y*u.x);}
    void normalize(){float n = sqrtf(x*x+y*y+z*z); x=x/n;y=y/n;z=z/n;}
    vec3 normalized(){float n = sqrtf(x*x+y*y+z*z); return vec3(x/n,y/n,z/n);}
    float norm(){float n = sqrtf(x*x+y*y+z*z); return n;}
    vec3 operator+(vec3 u){return vec3(x+u.x,y+u.y,z+u.z);}
    vec3 operator-(vec3 u){return vec3(x-u.x,y-u.y,z-u.z);}
    void operator+=(vec3 u){x+=u.x;y+=u.y;z+=u.z;}
    void operator-=(vec3 u){x-=u.x;y-=u.y;z-=u.z;}
    vec3 operator*(float f){return vec3(x*f,y*f,z*f);}
    float* value_ptr(){content[0]=x;content[1]=y;content[2]=z;return content;}
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

class vec4 {
public:
    float x{};
    float y{};
    float z{};
    float w = 1.f;
    vec4(){}
    vec4(float x, float y, float z) : x(x), y(y), z(z){}
    vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    ~vec4(){}
    vec4(vec3& v){x=v.x;y=v.y;z=v.z;w=1.f;}
    float operator[](int);
    float dot(vec4 u){return x*u.x+y*u.y+z*u.z+w*u.w;}
    float norm(){float n = sqrtf(x*x+y*y+z*z+w*w); return n;}
    vec4 normalized(){float n = sqrtf(x*x+y*y+z*z+w*w); return vec4(x/n,y/n,z/n,w/n);}
    void normalize(){float n = sqrtf(x*x+y*y+z*z+w*w); x=x/n;y=y/n;z=z/n;w=w/n;}
};

class mat4 {
private:
    float content[16]{};
public:
    float* value_ptr(){return content;}
    vec4 operator[](int);
    mat4(float);
    mat4(vec4,vec4,vec4,vec4);
    void print();
    float* get();
    mat4 operator*(mat4 A);
};

class mat4F {
public:
    static mat4 diagonal(vec4);
    static mat4 translation(vec3);
    // static mat4 rotation(float,vec3);
    static mat4 perspective(float,float,float,float);
    static mat4 lookAt(vec3,vec3,vec3);
};