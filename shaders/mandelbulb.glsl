#define MAX_ITER 24
#define BAILOUT 2.0

uniform float power;
uniform bool julia;
uniform vec3 juliaOffset;

vec2 DE(vec3 p) {
    vec3 c = julia ? juliaOffset : p;
    vec3 v = p;
    float dr = 1.0;             
    float r = length(v);  
    float trap = r;
    
    for (int i = 0; i < MAX_ITER; i++) {
        float r_pow_n_minus_one = pow(r, power - 1.0);
        float r_pow_n = r * r_pow_n_minus_one;
        float theta = atan(v.y, v.x) * power;
        float phi = asin(v.z / r) * power;
        dr = power * r_pow_n_minus_one * dr + 1.0;
        v = c + r_pow_n * vec3(cos(theta) * cos(phi), cos(phi) * sin(theta), -sin(phi));

        trap = min(trap, r);
        r = length(v);     
        if (r > BAILOUT) break;  
    }
    
    return vec2(0.5 * log(r) * r / dr, trap);
}



