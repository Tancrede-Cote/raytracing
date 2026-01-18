#version 330 core

in vec3 fragmentColor;
in vec3 fragPos;

out vec4 screenColor;

uniform vec3 camPos;

const float w = 1280.;
const float h = 1280.;
const vec3 omniLight = vec3(-0.8, 0.3, 0.2);

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
    // idea : spheric coordinates centered on center
    vec3 L = center-origin;
    float tca = dot(L,ray)/n(ray);// ray is supposed to be normalized
    if (tca<0.){
        return false;
    }
    float d = sqrt(n(L)*n(L)-tca*tca);
    if (d>r){
        return false;
    }
    return true;
}

vec3 intersect2(vec3 origin, vec3 ray, vec3 center, float r){
    // idea : spheric coordinates centered on center
    vec3 L = center-origin;
    float tca = dot(L,ray)/n(ray);// ray is supposed to be normalized
    if (tca<0.){
        return vec3(1.,0.,0.);
    }
    float d = sqrt(n(L)*n(L)-tca*tca);
    if (d>r){
        return vec3(0.,0.,1.);
    } else {
        float delta = 4*dot(origin,ray)*dot(origin,ray)-4*d2(ray,ray)*(d2(origin,origin)-r*r);
        if (delta<0.){
            return vec3(0.,1.,0.);
        }
        return origin+((-2*dot(origin,ray)-sqrt(delta))/(2*d2(ray,ray)))*ray;//sqrt(origin**2+a**2*ray**2)==r**2
    }
}

void main()
{
    vec3 ray = normalize(fragPos-camPos);
    if (intersect(camPos, ray, vec3(0.2, 0.3, -0.7), 0.4)){
        vec3 hit_point = intersect2(camPos, ray, vec3(0.2, 0.3, -0.7), 0.4);
        vec3 ray2 = omniLight-hit_point;
        screenColor = vec4((intersect(hit_point, ray2, vec3(0.2, 0.3, -0.7), 0.4))?vec3(d(hit_point,vec3(0.2, 0.3, -0.7)),0,0):vec3(1.,1.,1.),1.0);
        screenColor = vec4(1.0,1.0,0.,1.0);
    } else {
        screenColor = vec4((intersect(camPos, ray, vec3(0.2, 0.3, -0.7), 0.4))?vec3(1.,1.,0.):vec3(0.,0.,0.),1.0);
    }
}