#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColors;
layout(location = 2) in mat4 modelMatrix;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 oPosition;

void main() {
    oPosition = position;

    vec4 newPosition = uProjectionMatrix * uViewMatrix * modelMatrix * vec4(position, 1.0f);

    gl_Position = vec4(newPosition);
}