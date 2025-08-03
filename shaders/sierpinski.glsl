#define MAX_ITER 16

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