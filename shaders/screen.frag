#version 430 core
out vec4 fragColor;
in vec2 Texcoord;
layout (binding = 0) uniform sampler2D sceneTexture;

void main(){
    vec3 color = texture(sceneTexture, Texcoord).rgb;
    fragColor = vec4(color, 1.0);
}

