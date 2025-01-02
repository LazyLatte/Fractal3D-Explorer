#version 330 core
out vec4 fragColor;

#define PI 3.14159265
#define HALF_PI 1.570796
#define MAX_ITER 24
#define BAILOUT 64.0
#define RAY_STEP 500
#define FOV 1.0
#define FAR_PLANE 5.0

uniform float iTime;
uniform vec2 iResolution;
uniform vec3 pos;
uniform vec3 front;
uniform vec3 color_seed;
const float ray_multiplier = 0.8;

uniform uint power;
uniform float qk;
uniform bool julia;
uniform vec4 juliaOffset; // (w, x, y, z);

const float gloss = 8.0;
const vec3 lightColor = vec3(1.0);
const float lightIntensity = 1.0f;
const vec3  background1Color= vec3(0.125, 0.333, 0.470);
const vec3  background2Color= vec3(0.023, 0.109, 0.188); 
const float fogDensity= 0.4;  
const float decay = 1.0;
vec4 qMul(vec4 q1, vec4 q2){
    return vec4(
        q1.x * q2.x - q1.y * q2.y - q1.z * q2.z - q1.w * q2.w,
        q1.x * q2.y + q1.y * q2.x + q1.z * q2.w - q1.w * q2.z,
        q1.x * q2.z - q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
        q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x
    );
}
vec4 qSquare(vec4 q){
    return vec4(q.x*q.x - dot(q.yzw, q.yzw), 2.0 * q.x * q.yzw);
}
vec4 qCube(vec4 q){
    return q * (4.0 * q.x * q.x - dot(q, q) * vec4(3.0, 1.0, 1.0, 1.0));
}

vec4 qPow(vec4 q, uint pwr){
    if(pwr == 2u) return qSquare(q);
    if(pwr == 3u) return qCube(q);
    if(pwr == 4u) return qSquare(qSquare(q));
    if(pwr == 5u) return qMul(qSquare(q), qCube(q));
    if(pwr == 6u) return qSquare(qCube(q));
    if(pwr == 7u) return qMul(qSquare(qSquare(q)), qCube(q));
    if(pwr == 8u) return qSquare(qSquare(qSquare(q)));
    if(pwr == 9u) return qCube(qCube(q));
    return q;
}
vec2 DE(vec3 p) {
    vec4 v = vec4(p, qk);
    vec4 c = julia ? juliaOffset : v;
    float dr2 = 1.0f;             
    float r2 = dot(v, v);  
    float trap = r2;
    for (int i = 0; i < MAX_ITER; i++) {
        vec4 v_pow_minus_one = qPow(v, power - 1u);
        dr2 *= (power * power * dot(v_pow_minus_one, v_pow_minus_one));
        v = qMul(v_pow_minus_one, v) + c;
        
        r2 = dot(v, v);
        trap = min(trap, r2);
        if(r2 > BAILOUT) break;
    }
    return vec2(0.25f * log(r2) * sqrt(r2 / dr2), sqrt(trap));
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



    
