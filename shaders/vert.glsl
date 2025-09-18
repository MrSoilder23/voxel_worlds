#version 450 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoords;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 oPosition;
out vec3 oNormal;
out vec2 oTexCoords;
out vec4 oColor;

void main() {
    oPosition = position;
    oTexCoords = texCoords;


    vec4 newPosition = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(position, 1.0f);

    gl_Position = vec4(newPosition);
}