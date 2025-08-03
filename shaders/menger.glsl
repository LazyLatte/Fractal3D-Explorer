#define MAX_ITER 32
#define BAILOUT 2.0

uniform float xDeg;
uniform float yDeg;
uniform float zDeg;
uniform bool julia;
uniform vec3 juliaOffset;

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

float sdBox(vec3 p, vec3 b){
    vec3 d = abs(p) - b;
    return length(max(d, 0.0)) + min(max(d.x, max(d.y,d.z)), 0.0);
}

const float scale = 3.0f;
const vec3 offset = vec3(1.0f);
vec2 DE(vec3 p) {
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

    
