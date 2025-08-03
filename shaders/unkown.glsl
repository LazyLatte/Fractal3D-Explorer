uniform float param1;
uniform float param2;
uniform float param3;
uniform bool julia;
uniform vec3 juliaOffset;
// vec2 DE( vec3 p0 ){
//     vec4 p = vec4(p0, 1.0);
//     float trap = 10000.0;
//     for(int i = 0; i < 8; i++){
//         p.xyz = mod(p.xyz-1.0, 2.0) - 1.0;
//         trap = min(trap, length(p.xyz));
//         p*=1.4/dot(p.xyz,p.xyz);

        
//     }
//     return vec2(length(p.xyz/p.w) * 0.25, trap);
// }

// vec2 DE( vec3 p0 ){
//     float trap = 10000.0;
//     float dr = 1.0;
//     vec4 p = vec4(p0, 1.0);
//     for(int i = 0; i < 4; i++){        
//         p.xyz = mod(p.xyz-1.0, 2.0) - 1.0;
//         p.xyz -= sign(p.xyz)*0.036; // trick
//         float k = 0.844/dot(p.xyz, p.xyz);
//         p *= k;
//         dr *= k;

//         trap = min(trap, dot(p, p));
//     }
//     p /= p.w;
//     float d1 = sqrt( min( min( dot(p.xy,p.xy), dot(p.yz,p.yz) ), dot(p.zx,p.zx) ) ) - 0.57;
//     float d2 = dot(p.xyz, p.xyz);
//     float dmi = min(d1, d2);
//     return vec2(0.5 * dmi / dr, trap);
// }

	
vec2 DE( vec3 p ){
    float scale = 1.0;
    float trap = 10000.0;
    for(int i=0; i<6; i++){
        p = mod(p - 1.0, 2.0) - 1.0;
        p -= sign(p) * 0.04;
        float r2 = dot(p,p);
        float k = 0.95/r2;
        p  *= k;  scale *= k;
        trap = min(trap, r2);
    }

    float d1 = sqrt( min( min( dot(p.xy,p.xy), dot(p.yz,p.yz) ), dot(p.zx,p.zx) ) ) - 0.02;
    float d2 = abs(p.y);
    float dmi = d2;
    if( d1 < d2 ) dmi = d1;
    return vec2(0.5 * dmi / scale, trap);
}

// vec2 DE( vec3 p ){
//     p.z-=2.5;
//     float s = 3.;
//     float e = 0.;
//     float trap = 10000.0;
//     for(int j=0;j++<8;){
//         s*=e=3.8/clamp(dot(p,p),0.,2.),
//         p=abs(p)*e-vec3(1,15,1);
//         trap = min(trap, length(p));
//     }

//     return vec2(length(cross(p,vec3(1,1,-1)*.577))/s, trap);
// }

// vec2 DE(vec3 p){
//     float s=2., l=0.;
//     p=abs(p);
//     float trap = 10000.0;
//     for(int j=0;j++<8;){
//         p=-sign(p)*(abs(abs(abs(p)-2.)-1.)-1.),
//         p*=l=-1.3/dot(p,p),
//         p-=.15, s*=l;
//         trap = min(trap, length(p));
//     }

//     return vec2(length(p)/s, trap);
// }

// vec2 DE(vec3 p){
//     p.xz=mod(p.xz,2.)-1.;
//     float s=2., e;
    
//     float trap = 10000.0;
//     for(int j=0;j++<8;){
//         s*=e=2./clamp(dot(p,p),.5,1.),
//         p=abs(p)*e-vec3(.5,8,.5);
//         trap = min(trap, length(p));
//     }

//     return vec2(length(p.xz)/s, trap);
// }

// vec2 DE(vec3 p){
//     p=fract(p)-.5;
//     float s=3., l;
//     float trap = 10000.0;
//     for(int j=0;j++<8;){
//       p=abs(p),
//       p=p.x<p.y?p.zxy:p.zyx,
//       s*=l=2./min(dot(p,p),1.),
//       p=p*l-vec3(.2,1,4);
//       trap = min(trap, length(p));
//     }

//     return vec2(length(p)/s, trap);
// }


// vec2 DE( vec3 p ) {
//   float e, s, t=0.0; // time adjust term
//   vec3 q=p;
//   p.z+=7.;
//   p=vec3(log(s=length(p)),atan(p.y,p.x),sin(t/4.+p.z/s));
//   s=1.;
//   float trap =10000.0;
//   for(int j=0;j++<6;){
//     s*=e=PI/min(dot(p,p),.8),
//     p=abs(p)*e-3.,
//     p.y-=round(p.y);
//     trap = min(trap, length(p));
//   }

//   return vec2(length(p)/s, trap);
// }