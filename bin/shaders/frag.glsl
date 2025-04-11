#version 450 core
#extension GL_ARB_gpu_shader_int64 : enable
#extension GL_ARB_bindless_texture : require

layout(std430, binding = 0) buffer TextureHandles {
    uint64_t textureHandles[];
};

in vec3 oPosition;
flat in uint oTexID;
in vec3 oTexCoords;

out vec4 color; 

void main() { 
    samplerCube cubeTexture = samplerCube(textureHandles[oTexID]);

    color = texture(cubeTexture, oTexCoords);
}