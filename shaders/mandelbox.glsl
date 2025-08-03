#define MAX_ITER 16
#define BAILOUT2 64.0

uniform float scale;
uniform float minR;
uniform float fold;
uniform bool julia;
uniform vec3 juliaOffset;

vec2 DE(vec3 p) {
    float minRadius2 = minR * minR;
    float fixedRadius2 = 1.0;
    
    vec3 c = julia ? juliaOffset : p;
    vec3 v = p;
    float r2; 
    float dr = 1.0;
    float md2 = 1000.0;

    for(int i=0; i<MAX_ITER; i++){
        v = clamp(v, -1.0, 1.0) * 2.0 - v;
        v *= fold;
        r2 = dot(v, v);
        if(r2 < minRadius2){
            float factor = fixedRadius2 / minRadius2;
            v *= factor;
            dr *= factor;
        }else if(r2 < fixedRadius2){
            float factor = fixedRadius2 / r2;
            v *= factor;
            dr *= factor;
        }

        v = c + scale * v;
        dr = dr * abs(scale) + 1.0;
        md2 = min(md2, r2);
        if(r2 > BAILOUT2) break;
    }
    return vec2(sqrt(r2) / abs(dr), sqrt(md2));
}