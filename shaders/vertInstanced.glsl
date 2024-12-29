#version 450 core
#extension GL_ARB_gpu_shader_int64 : enable
#extension GL_ARB_bindless_texture : require

layout(location = 0) in vec3 position;
layout(location = 2) in mat4 modelMatrix;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 oPosition;
flat out int instanceID;

void main() {
    oPosition = position;
    instanceID = gl_InstanceID;
    
    vec4 newPosition = uProjectionMatrix * uViewMatrix * modelMatrix * vec4(position, 1.0f);

    gl_Position = vec4(newPosition);
}