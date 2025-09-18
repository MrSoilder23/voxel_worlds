#version 450 core

in vec2 oTexCoords;

out vec4 color; 

uniform sampler2D uTexture;
// uniform bool uUseTexture = true;
// uniform bool uUseVertexColor = false;

void main() {
    color = texture(uTexture, oTexCoords);
}