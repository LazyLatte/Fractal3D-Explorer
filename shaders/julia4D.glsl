#define MAX_ITER 24
#define BAILOUT 8.0

uniform float power;
uniform float qk;
uniform bool julia;
uniform vec4 juliaOffset; // (w, x, y, z);

vec2 DE(vec3 p) {
    vec4 q = vec4(p, qk);
    vec4 c = julia ? juliaOffset : q;
    float dr = 1.0f;             
    float r = length(q);  
    float trap = r;
    for (int i = 0; i < MAX_ITER; i++) {
        float theta = atan(length(q.yzw), q.x) * power;
        q = c + pow(r, power) * vec4(cos(theta), sin(theta) * vec3(q.yzw) / r);
        dr = power * pow(r, power - 1.0) * dr + 1.0;
        
        r = length(q);  
        trap = min(trap, r);
        if(r > BAILOUT) break;
    }
    return vec2(0.5f * log(r) * r / dr, trap);
}