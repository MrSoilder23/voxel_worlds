#version 410 core

in vec3 oPosition;
in vec2 oTexCoord;

uniform sampler2D uTextureAtlas;

out vec4 color;

void main() { 
    color = texture(uTextureAtlas, oTexCoord);
}