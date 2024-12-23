#version 410 core

in vec3 oPosition;
uniform samplerCube uCubeTextureSample;

out vec4 color;

void main() {
    color = texture(uCubeTextureSample, oPosition);
}