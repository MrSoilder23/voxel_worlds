#version 410 core

in vec3 oVertexColors;
uniform samplerCube uCubeTextureSample;

out vec4 color;

void main() {
    color = texture(uCubeTextureSample, oVertexColors);
}