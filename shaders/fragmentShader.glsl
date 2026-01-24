#version 330 core

in vec3 fragmentColor;
in vec3 fragPos;

out vec4 screenColor;

uniform vec3 camPos;
uniform float time;

uniform int w;
uniform int h;


float d(vec3 u, vec3 v){
    return sqrt((u.x-v.x)*(u.x-v.x)+(u.y-v.y)*(u.y-v.y)+(u.z-v.z)*(u.z-v.z));
}

float d2(vec3 u, vec3 v){
    return (u.x-v.x)*(u.x-v.x)+(u.y-v.y)*(u.y-v.y)+(u.z-v.z)*(u.z-v.z);
}

float n(vec3 u){
    return d(u, vec3(0.,0.,0.));
}

// with a sphere
bool intersect(vec3 origin, vec3 ray, vec3 center, float r){
    vec3 L = center-origin;
    float tca = dot(L,ray)/n(ray);
    if (tca<0.){
        return false;
    }
    float d = sqrt(n(L)*n(L)-tca*tca);
    if (d>r){
        return false;
    }
    return true;
}

vec3 intersectp(vec3 origin, vec3 ray, vec3 center, vec3 normal){
    // dot(origin+a*ray-center,normal) = 0
    // dot(origin-center,normal)+a*dot(ray,normal) = 0
    float t0 = -dot(origin-center,normal)/dot(ray,normal);
    if(t0>0){
        return origin+t0*ray;
    }
    return vec3(0,0,0);
}

vec3 intersect2(vec3 origin, vec3 ray, vec3 center, float r){
    vec3 L = center-origin;
    float tca = dot(L,ray)/n(ray);
    if (tca<0.){
        return vec3(0.,0.,0.);
    }
    float d = sqrt(n(L)*n(L)-tca*tca);
    if (d>r){
        return vec3(0.,0.,0.);
    } else {
        float t0 = tca - sqrt(r*r-d*d);
        return origin+t0*ray;
    }
}

float intersect3(vec3 origin, vec3 ray, vec3 center, float r){
    vec3 L = center-origin;
    float tca = dot(L,ray)/n(ray);
    if (tca<0.){
        return 0.;
    }
    float d = sqrt(n(L)*n(L)-tca*tca);
    if (d>r){
        return 0.;
    } else {
        float t0 = tca - sqrt(r*r-d*d);
        return t0;
    }
}

// Returns .x > .y if no intersection
vec2 raySphere(vec3 rayPos, vec3 rayDir, vec3 sphPos, float radius)
{
    vec3 p = rayPos - sphPos;
    float delta = 4. * (dot(p, rayDir) * dot(p, rayDir) - dot(rayDir, rayDir) * (dot(p, p) - radius * radius));
    if(delta < 0.) return vec2(1e5, -1e5);
    return vec2((-2. * dot(p, rayDir) - sqrt(delta)) / (2. * dot(rayDir, rayDir)), 
                (-2. * dot(p, rayDir) + sqrt(delta)) / (2. * dot(rayDir, rayDir)));
}

mat2 rot2D(float theta)
{
    return mat2(vec2(cos(theta), -sin(theta)), vec2(sin(theta), cos(theta)));
}

void main()
{
    vec3 sphereCenter = vec3(0., 0.5, 0.);
    vec3 omniLight = vec3(0.5, 1.3, -0.5);
    omniLight.xz *= rot2D(time);
    vec3 uv = vec3(fragPos.x*w/h,fragPos.y,fragPos.z);
    float fov = 70.;
    vec3 ray = vec3(uv.xy, -2. / tan(fov));
    vec3 hit_point = intersect2(camPos, ray, sphereCenter, 0.4);
    vec3 hit_point2 = intersectp(camPos, ray, vec3(0,0,0), vec3(0,1,0));
    vec3 hit_point3 = intersect2(camPos, ray, omniLight, 0.4);
    bool touch_sphere = false;
    screenColor = vec4(0.,0.25,1.,1.);
    if(length(hit_point2)>0.){
        screenColor = vec4(0.5,0.5,0.5,1.);
        if (intersect(hit_point2, normalize(omniLight-hit_point2), sphereCenter, 0.4)){
            screenColor = vec4(0,0,0,1);
        }
    } 
    if (length(hit_point)>0.){
        touch_sphere=true;
        vec3 ray2 = omniLight-hit_point;
        //screenColor = vec4(1.,1.,0.,1.);
        screenColor = vec4((intersect(omniLight,-ray2,sphereCenter,0.4))?vec3(1.,0.5,0.):vec3(1.,1.,1.),1.0);
    } else {
        
        // screenColor = vec4((intersect(hit_point2, normalize(omniLight-hit_point2), sphereCenter, 0.4))?vec3(0,0,0):vec3(1,0,0.),1.0);
    }
    if (length(hit_point3)>0.){
        screenColor = vec4(vec3(1.,1.,0.),1.0);
    } else {
        // screenColor = vec4(1.0,1.0,0.,1.0);
    }
}