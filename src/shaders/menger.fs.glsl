#version 330 core
out vec4 fragColor;

#define PI 3.14159265
#define HALF_PI 1.570796
#define MAX_ITER 32
#define BAILOUT 2.0
#define RAY_STEP 150
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
const vec3  background1Color= vec3(0.125, 0.333, 0.470);
const vec3  background2Color= vec3(0.023, 0.109, 0.188); 
const float fogDensity= 0.4;  
const float decay = 1.0;

vec3 rotateX(vec3 v, float deg){
    float rad = deg * PI / 180.0;
    float c = cos(rad);
    float s = sin(rad);
    mat3 rotateMatrix = mat3(
        vec3(1, 0, 0),
        vec3(0, c, -s),
        vec3(0, s, c)
    );
    return rotateMatrix * v;
}
vec3 rotateY(vec3 v, float deg){
    float rad = deg * PI / 180.0;
    float c = cos(rad);
    float s = sin(rad);
    mat3 rotateMatrix = mat3(
        vec3(c, 0, s),
        vec3(0, 1, 0),
        vec3(-s, 0, c)
    );
    return rotateMatrix * v;
}
vec3 rotateZ(vec3 v, float deg){
    float rad = deg * PI / 180.0;
    float c = cos(rad);
    float s = sin(rad);
    mat3 rotateMatrix = mat3(
        vec3(c, -s, 0),
        vec3(s, c, 0),
        vec3(0, 0, 1)
    );
    return rotateMatrix * v;
}

float sdBox(vec3 p, vec3 b){
    vec3 d = abs(p) - b;
    return length(max(d, 0.0)) + min(max(d.x, max(d.y,d.z)), 0.0);
}

vec2 DE(vec3 p) {
    const float scale = 3.0f;
    const vec3 offset = vec3(1.0f);
    vec3 c = julia ? juliaOffset : vec3(0.0f);

    vec3 v = p;
    float s = 1.0;
    float trap = dot(v,v);
    for(int i=0; i<MAX_ITER; i++){
        v = abs(v);  
        if(v.x > BAILOUT || v.y > BAILOUT || v.z > BAILOUT) break;
        if(v.x < v.y) v.xy = v.yx;
        if(v.x < v.z) v.xz = v.zx;
        if(v.y < v.z) v.yz = v.zy;
        s *= scale;
        v = v * scale - offset * (scale - 1.0);
        float zVal = offset.z * (scale - 1.0);
        if(v.z < -0.5 * zVal) v.z += zVal;
        v = rotateX(v, xDeg);
        v = rotateY(v, yDeg);
        v = rotateZ(v, zDeg);
        v += c;
        trap = min(trap, dot(v,v)); 
    }
    return vec2(sdBox(v,vec3(1.0)) / s, sqrt(trap));
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
        eps = dist * 0.002f;
        if(abs(est) < eps || dist > FAR_PLANE) break;
        dist += est * ray_multiplier;
    }
    vec3 lightPos = pos;
    vec3 color = vec3(0.0);
    vec4 bg_color = vec4(clamp(mix(background2Color, background1Color, (sin(ray_dir.y * HALF_PI) + 1.0) * 0.5), 0.0, 1.0), 1.0);
    if(dist < FAR_PLANE){
        vec3 v = pos + dist * ray_dir;
        vec3 N = normalize(vec3(
            DE(v + vec3(eps, 0.0, 0.0))[0] - DE(v - vec3(eps, 0.0, 0.0))[0],
            DE(v + vec3(0.0, eps, 0.0))[0] - DE(v - vec3(0.0, eps, 0.0))[0],
            DE(v + vec3(0.0, 0.0, eps))[0] - DE(v - vec3(0.0, 0.0, eps))[0]
        ));

        vec3 L = normalize(lightPos - v);
        vec3 H = normalize(L - ray_dir);

        vec3 ambient = mix(background2Color, background1Color, (0.7f + 0.3f * N.y)) * (0.2f + 0.8f * clamp(0.05f * log(orbit), 0.0f, 1.0f));
        vec3 diffuse = lightColor * lightIntensityDecay(lightIntensity, dot(lightPos - v, lightPos - v)) * clamp(dot(L, N), 0.0, 1.0);
        vec3 specular = diffuse * pow(clamp(dot(H, N), 0.0, 1.0), gloss);
        
        color = palette(orbit, color_seed);

        color *= (ambient + diffuse);
        color += specular * 0.8f;
        //color /= (orbit * orbit);
        color = fog(bg_color.rgb, color, dist);
    }else {
        color = bg_color.rgb;
    }
    color = gammaCorrection(color);
    fragColor = vec4(color, 1.0);
}



    
