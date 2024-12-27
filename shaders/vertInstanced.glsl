#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in mat4 modelMatrix;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 oPosition;
out vec2 oTexCoord;

void main() {
    oPosition = position;
    oTexCoord = texCoord;

    vec4 newPosition = uProjectionMatrix * uViewMatrix * modelMatrix * vec4(position, 1.0f);

    gl_Position = vec4(newPosition);
}