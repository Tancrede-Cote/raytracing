#version 330 core

in vec3 fragmentColor;
in vec3 fragPos;

out vec4 screenColor;

uniform vec3 camPos;
uniform vec3 pos;
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

// Returns .x > .y if no intersection
vec2 rayPlan(vec3 origin, vec3 ray, vec3 center, vec3 normal, out vec3 res){
    // dot(origin+a*ray-center,normal) = 0
    // dot(origin-center,normal)+a*dot(ray,normal) = 0
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

mat2 rot2D(float theta)
{
    return mat2(vec2(cos(theta), -sin(theta)), vec2(sin(theta), cos(theta)));
}

void main()
{
    vec3 sphereCenter = vec3(sin(time/3), 0.5, 0.);
    vec3 sphereColor = vec3(1, 0., 0.);
    int nSpheres = 2;
    vec3 aSpheres[2] = vec3[](vec3(sin(time/3), 0.5, 0.),vec3(-0.5,0.,-0.5));
    int nLights = 2;
    vec3 alights[2] = vec3[](vec3(sin(time), 1.3, -1.),vec3(1., sin(time)/2+1, -1.));
    vec3 omniLight = vec3(sin(time), 1.3, -1.);
    omniLight.xz *= rot2D(time);
    alights[0].xz *= rot2D(time);
    vec3 uv = vec3(fragPos.x*w/h,fragPos.y,fragPos.z);
    float fov = 70.;
    vec3 ray = vec3(uv.xy, -2. / tan(fov));
    vec3 hit_point;
    
    vec2 inte = raySphere(camPos, ray, pos, 0.4, hit_point);
    vec3 hit_point2;
    rayPlan(camPos, ray, vec3(0,0,0), vec3(0,1,0), hit_point2);
    vec2 t = raySphere(camPos, ray, alights[0], 0.4);
    bool ground = length(hit_point2)>0.;
    bool sphere = inte.x<inte.y;
    bool light = false;
    float light_d = 30000;
    vec3 hit_light;

    bool transparent = false;


    for(int i=0; i<nLights; i++){
        vec2 t = raySphere(camPos, ray, alights[i], 0.4,hit_light);
        light=light||(t.x<t.y);
        if (t.x<t.y){
            light_d=min(light_d,d(camPos,hit_light));
        }
    }
    bool closest = (light_d<d(hit_point, camPos));// true if a light is closer than the sphere
    bool closest2 = (ground)&&(d(hit_point2,camPos)<d(hit_point, camPos));// true if a plane is closer than the sphere
    float d_to_plan = d(hit_point,hit_point2);// distance plane/sphere, TODO : adapt for multiple planes/spheres
    vec3 ambient = vec3(0.,0.25,1.);
    if (ground){
        ambient = hit_point2.z<-10?vec3(0.,0.25,1):(int(floor(hit_point2.x)+floor(hit_point2.z))%2==0)?vec3(0.2,0.2,0.2):vec3(0.5,0.5,0.5);
    } if (light){
        ambient = vec3(.8,.8,0);
        ground = false;
    } if (sphere  && !closest2 && !(transparent)){
        ambient = sphereColor*vec3(0.02,0.02,0.02);
        ground = false;
    } 
    screenColor = vec4(ambient,1.);
    for(int i=0; i<nLights; i++){
        if(ground){
            vec3 hp;
            vec2 temp = raySphere(hit_point2, (alights[i]-hit_point2), pos, 0.4, hp);
            if (hit_point2.z>-10 && temp.x<temp.y && d(hit_point2,pos)<d(hit_point2,alights[i])){
                screenColor -= vec4(.2,.2,.2,1);
            }
        } 
        if (sphere){
            vec3 ray2 = alights[i]-hit_point;
            vec3 ray3 = hit_point-pos;
            
            float dis = (dot(normalize(ray2),normalize(ray3))+1)/2;
            float di = (d2(pos,hit_point));
            if (hit_point.y>=0.){
                screenColor += vec4(vec3(dis)*sphereColor*0.5,1.);
            }
        } 
    }
    if (closest){
        screenColor = vec4(.8,.8,0.,1.);
    }
}