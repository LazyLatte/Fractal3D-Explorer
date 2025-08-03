#version 430 core
const vec2[4] positions = vec2[4](
    vec2(-1.0, -1.0), 
    vec2(1.0, -1.0),
    vec2(1.0, 1.0),
    vec2(-1.0, 1.0)
);

out vec2 Texcoord;

void main(){
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
    Texcoord = positions[gl_VertexID] * 0.5 + 0.5;
}