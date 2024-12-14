#version 410 core

in vec3 oVertexColors;

out vec4 color;

void main() {
    color = vec4(oVertexColors, 1.0f);
}