#version 330 core

in vec3 fragmentColor;
in vec3 fragPos;

out vec4 screenColor;

uniform vec3 camPos;
uniform vec3 pos;
uniform float time;
uniform vec3 plane;
uniform float r;
uniform float lr;
uniform vec3 a;
uniform vec3 b;
uniform vec3 c;
vec3 ma = vec3(3, 0, -2);
vec3 mb = vec3(3, 2, 2);
vec3 mc = vec3(3, 2, -2);
vec3 md = vec3(3, 0, 2);


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

// Returns .x > .y if no intersection
vec2 rayPlan(vec3 origin, vec3 ray, vec3 center, vec3 normal, out vec3 res){
    float t0 = -dot(origin-center,normal)/dot(ray,normal);
    if(t0>0){
        res = origin+t0*ray;
        return vec2(-t0,t0);
    }
    res = vec3(0,0,0);
    return vec2(1e5,-1e5);
}

float d(vec3 center, float r, vec3 origin, vec3 normal){ // sphere/plane distance
    vec3 projection;
    vec2 temp = rayPlan(center,-normal,origin,normal,projection);
    if (temp.x>temp.y){
        vec2 temp = rayPlan(center,normal,origin,normal,projection);
    }
    return max(0,d(center,projection)-r);
}

// Returns .x > .y if no intersection
vec2 raySphere(vec3 rayPos, vec3 rayDir, vec3 sphPos, float radius, out vec3 point)
{
    vec3 p = rayPos - sphPos;
    float delta = 4. * (dot(p, rayDir) * dot(p, rayDir) - dot(rayDir, rayDir) * (dot(p, p) - radius * radius));
    if(delta < 0.) {
        point = vec3(0,0,0); 
        return vec2(1e5, -1e5);
    }
    point = rayPos+(-2. * dot(p, rayDir) - sqrt(delta)) / (2. * dot(rayDir, rayDir))*rayDir;
    return vec2((-2. * dot(p, rayDir) - sqrt(delta)) / (2. * dot(rayDir, rayDir)), 
                (-2. * dot(p, rayDir) + sqrt(delta)) / (2. * dot(rayDir, rayDir)));
}

vec2 raySphere(vec3 rayPos, vec3 rayDir, vec3 sphPos, float radius)
{
    vec3 p = rayPos - sphPos;
    float delta = 4. * (dot(p, rayDir) * dot(p, rayDir) - dot(rayDir, rayDir) * (dot(p, p) - radius * radius));
    if(delta < 0.) return vec2(1e5, -1e5);
    return vec2((-2. * dot(p, rayDir) - sqrt(delta)) / (2. * dot(rayDir, rayDir)), 
                (-2. * dot(p, rayDir) + sqrt(delta)) / (2. * dot(rayDir, rayDir)));
}

vec3 raySphere2(vec3 rayPos, vec3 rayDir, vec3 sphPos, float radius)
{
    vec3 p = rayPos - sphPos;
    float delta = 4. * (dot(p, rayDir) * dot(p, rayDir) - dot(rayDir, rayDir) * (dot(p, p) - radius * radius));
    if(delta < 0.) return vec3(1e5, -1e5, 0);
    return rayPos+(-2. * dot(p, rayDir) - sqrt(delta)) / (2. * dot(rayDir, rayDir))*rayDir;
}

float area(vec3 a, vec3 b, vec3 c){// area of triangle abc
    return 0.5*length(cross(b-a,c-a));
}

vec2 rayTriangle(vec3 origin, vec3 ray, vec3 a, vec3 b, vec3 c, out vec3 res){
    vec3 normal = normalize(cross(b-a,c-a));
    float t0 = -dot(origin-a,normal)/dot(ray,normal);
    if(t0>0){
        res = origin+t0*ray;
        float l1 = area(a,b,res)/area(a,b,c);
        float l2 = area(a,c,res)/area(a,b,c);
        float l3 = area(c,b,res)/area(a,b,c);
        if (l1+l2+l3>1.01){
            res = vec3(0,0,0);
            return vec2(1e5,-1e5);
        }
        return vec2(-t0,t0);
    }
    res = vec3(0,0,0);
    return vec2(1e5,-1e5);
}

mat2 rot2D(float theta)
{
    return mat2(vec2(cos(theta), -sin(theta)), vec2(sin(theta), cos(theta)));
}

void main()
{

    vec3 sphereColor = vec3(1, 0., 0.);
    vec3 triangleColor = vec3(0.2,0.7,0.2);
    int nLights = 2;
    vec3 alights[2] = vec3[](vec3(sin(time), 1.5, -1.),vec3(1., sin(time)/2+1.5, -1.));
    alights[0].xz *= rot2D(time);
    vec3 uv = vec3(fragPos.x*w/h,fragPos.y,fragPos.z);
    float fov = 70.;
    vec3 ray = vec3(uv.xy, -2. / tan(fov));
    vec3 hit_point;
    
    vec2 inte = raySphere(camPos, ray, pos, r, hit_point);
    vec3 hit_point2;
    vec2 p = rayPlan(camPos, ray, vec3(0,-1,0), plane, hit_point2);
    vec2 t = raySphere(camPos, ray, alights[0], r);
    bool ground = p.x<p.y;
    bool sphere = inte.x<inte.y&&hit_point.z<=3;
    bool light = false;
    float light_d = 30000;
    vec3 hit_light;

    bool transparent = false;

    vec3 triangle_hit;
    vec2 tri_inter = rayTriangle(camPos,ray,a,b,c,triangle_hit);

    bool triangle = tri_inter.x<tri_inter.y;
    for(int i=0; i<nLights; i++){
        vec2 t = raySphere(camPos, ray, alights[i], lr,hit_light);
        light=light||(t.x<t.y);
        if (t.x<t.y){
            light_d=min(light_d,d(camPos,hit_light));
        }
    }
    bool closest = (light_d<d(hit_point, camPos));// true if a light is closer than the sphere
    bool closest2 = (ground)&&(d(hit_point2,camPos)<d(hit_point, camPos));// true if a plane is closer than the sphere
    vec3 ambient = vec3(0.,0.25,1.);
    if (ground){
        ambient = hit_point2.z<-20?vec3(0.,0.25,1):(int(floor(hit_point2.x)+floor(hit_point2.z))%2==0)?vec3(0.2,0.2,0.2):vec3(0.5,0.5,0.5);
    } if (light){
        ambient = vec3(.8,.8,0);
        ground = false;
    } 
    // if (triangle){ // ## CODE TO RENDER TRIANGLE ##
    //     ambient = triangleColor;
    // } 
    if (sphere  && !closest2 && !(transparent)){
        ambient = sphereColor*vec3(0.02,0.02,0.02);
        ground = false;
    } 
    screenColor = vec4(ambient,1.);
    
    for(int i=0; i<nLights; i++){
        if(ground){
            vec3 hp;
            vec2 temp = raySphere(hit_point2, (alights[i]-hit_point2), pos, r, hp);
            if (hit_point2.z>-20 && temp.x<temp.y && d(hit_point2,pos)<d(hit_point2,alights[i])){
                screenColor -= vec4(.2,.2,.2,1);
            }
            // vec2 temp2 = rayTriangle(hit_point2,(alights[i]-hit_point2),a,b,c,hp); // ## CODE TO RENDER TRIANGLE ##
            // if (!triangle && hit_point2.z>-10 && temp2.x<temp2.y && d(hit_point2,pos)<d(hit_point2,alights[i])){
            //     screenColor -= vec4(.2,.2,.2,1);
            // }
        } 
        // if(triangle&&!sphere){ // ## CODE TO RENDER TRIANGLE ##
        //     vec3 ray2 = alights[i]-hit_point;
        //     vec3 ray3 = hit_point-pos;
            
        //     float dis = (dot(normalize(ray2),normalize(ray3))+1)/2;
        //     screenColor += vec4(vec3(dis)*triangleColor,1.);
        // }
        if (sphere){
            vec3 ray2 = alights[i]-hit_point;
            vec3 ray3 = hit_point-pos;
            
            float dis = (dot(normalize(ray2),normalize(ray3))+1)/2;
            if (!closest2){
                screenColor += vec4(vec3(dis)*sphereColor*0.5,1.);
            }
        } 
    }
    if (closest){
        screenColor = vec4(.8,.8,0.,1.);
    }
}