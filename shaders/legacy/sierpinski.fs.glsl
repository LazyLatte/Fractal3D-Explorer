#version 330 core
out vec4 fragColor;

#define PI 3.14159265
#define HALF_PI 1.570796
#define MAX_ITER 16
#define RAY_STEP 100
#define FOV 1.0
#define FAR_PLANE 5.0

uniform float iTime;
uniform vec2 iResolution;
uniform vec3 pos;
uniform vec3 front;
uniform vec3 color_seed;
const float ray_multiplier = 1.0;

uniform float xDeg;
uniform float yDeg;
uniform float zDeg;
uniform bool julia;
uniform vec3 juliaOffset;

const float gloss = 8.0;
const vec3 lightColor = vec3(1.0);
const float lightIntensity = 1.0f;
const float fogDensity= 0.4;  
const float decay = 1.0;

vec3 rotateX(vec3 v, float deg){
    float rad = deg * PI / 180.0;
    float c = cos(rad);
    float s = sin(rad);
    mat3 rotateMatrix = mat3(
        vec3(1, 0, 0),
        vec3(0, c, s),
        vec3(0, -s, c)
    );
    return rotateMatrix * v;
}
vec3 rotateY(vec3 v, float deg){
    float rad = deg * PI / 180.0;
    float c = cos(rad);
    float s = sin(rad);
    mat3 rotateMatrix = mat3(
        vec3(c, 0, -s),
        vec3(0, 1, 0),
        vec3(s, 0, c)
    );
    return rotateMatrix * v;
}
vec3 rotateZ(vec3 v, float deg){
    float rad = deg * PI / 180.0;
    float c = cos(rad);
    float s = sin(rad);
    mat3 rotateMatrix = mat3(
        vec3(c, s, 0),
        vec3(-s, c, 0),
        vec3(0, 0, 1)
    );
    return rotateMatrix * v;
}

vec2 DE(vec3 p){
    const float scale = 2.0f;
    const vec3 offset = vec3(1.0f);
    vec3 c = julia ? juliaOffset : vec3(0.0f);
    vec3 v = p;
    float trap = dot(v, v);
    for(int i=0; i<MAX_ITER; i++){
        if(v.x+v.y<0.0f) v.xy = -v.yx; 
        if(v.x+v.z<0.0f) v.xz = -v.zx;
        if(v.y+v.z<0.0f) v.yz = -v.zy; 
        v = v*scale - offset*(scale-1.0f);
        v = rotateX(v, xDeg);
        v = rotateY(v, yDeg);
        v = rotateZ(v, zDeg);
        v += c;
        trap = min(dot(v, v), trap);
    }
    return vec2(length(v)* pow(scale, -float(MAX_ITER)), sqrt(trap));
}

vec3 palette(float t, vec3 d){
    vec3 a = vec3(0.25);
    vec3 b = vec3(0.25);
    vec3 c = vec3(0.5);
    return a + b * cos(2.0 * PI * (c * t + d));
}

vec3 lightIntensityDecay(float intensity, float dist2){
    return intensity / (1.0 + decay*dist2);
}

vec3 fog(vec3 fogColor, vec3 color, float dist){
    return mix(fogColor, color, clamp(1.0 / exp(dist * fogDensity), 0.0, 1.0));
}

vec3 gammaCorrection(vec3 color){
    float gamma = 0.4545;
    return vec3(pow(color.r, gamma), pow(color.g, gamma), pow(color.b, gamma));
}


void main(){
    vec2 uv = (gl_FragCoord.xy * 2.0 - iResolution) / iResolution.y;
    vec3 right = cross(front, vec3(0.0, 1.0, 0.0));
    vec3 up = cross(right, front);
    vec3 ray_dir = normalize(uv.x * right + uv.y * up + FOV * front);

    float dist = 0.0;
    float orbit = 0.0;
    float eps;
    for(int i=0; i<RAY_STEP; i++){
        vec2 map = DE(pos + dist * ray_dir);
        float est = map.x;
        orbit = map.y;
        eps = dist * 0.003f;
        if(abs(est) < eps || dist > FAR_PLANE) break;
        dist += est * ray_multiplier;
    }
    vec3 lightPos = pos;
    vec3 color = vec3(0.0);
    if(dist < FAR_PLANE){
        vec3 v = pos + dist * ray_dir;
        vec3 N = normalize(vec3(
            DE(v + vec3(eps, 0.0, 0.0))[0] - DE(v - vec3(eps, 0.0, 0.0))[0],
            DE(v + vec3(0.0, eps, 0.0))[0] - DE(v - vec3(0.0, eps, 0.0))[0],
            DE(v + vec3(0.0, 0.0, eps))[0] - DE(v - vec3(0.0, 0.0, eps))[0]
        ));

        vec3 L = normalize(lightPos - v);
        vec3 H = normalize(L - ray_dir);

        float ambc = (0.7f + 0.3f * N.y) * (0.2f + 0.8f * clamp(0.05f * log(orbit), 0.0f, 1.0f));
        vec3 ambient = vec3(0.3);
        vec3 diffuse = lightColor * lightIntensityDecay(lightIntensity, dot(lightPos - v, lightPos - v)) * clamp(dot(L, N), 0.0, 1.0);
        vec3 specular = diffuse * pow(clamp(dot(H, N), 0.0, 1.0), gloss);
        
        color = palette(orbit, color_seed);

        color *= (ambc * ambient + diffuse);
        color += specular * 0.8f;
        color /= (orbit * orbit);
    }
    color = gammaCorrection(color);
    fragColor = vec4(color, 1.0);
}



    
