#include "./blocks/block_registry.hpp"

void BlockRegistry::registerBlock(
    BlockTypes    type,
    MeshComponent mesh
) {
    const size_t index = static_cast<size_t>(type);
    if (index >= NUM_BLOCK_TYPES) {
        throw std::out_of_range("Invalid BlockType");
    }

    mMesh[index] = std::move(mesh);
}

const MeshComponent& BlockRegistry::getMesh(BlockTypes type) const {
    const size_t index = static_cast<size_t>(type);    
    if (index >= NUM_BLOCK_TYPES) {
        static MeshComponent defaultMesh;
        return defaultMesh;
    }

    return mMesh[index];
}

MeshComponent BlockRegistry::createCubeMesh(const std::array<glm::vec2, 6>& faceTexCoords) {
    MeshComponent mesh;

    // positions, normals, and texture coordinates
    mesh.vertices = {
        // Front face
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), faceTexCoords[0], glm::vec4(1.0f)},
        {glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), faceTexCoords[0], glm::vec4(1.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), faceTexCoords[0], glm::vec4(1.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), faceTexCoords[0], glm::vec4(1.0f)},
        
        // Back face
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), faceTexCoords[1], glm::vec4(1.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), faceTexCoords[1], glm::vec4(1.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), faceTexCoords[1], glm::vec4(1.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), faceTexCoords[1], glm::vec4(1.0f)},
        
        // Right face
        {glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), faceTexCoords[2], glm::vec4(1.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), faceTexCoords[2], glm::vec4(1.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), faceTexCoords[2], glm::vec4(1.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), faceTexCoords[2], glm::vec4(1.0f)},
        
        // Left face
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), faceTexCoords[3], glm::vec4(1.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), faceTexCoords[3], glm::vec4(1.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), faceTexCoords[3], glm::vec4(1.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), faceTexCoords[3], glm::vec4(1.0f)},
        
        // Top face
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), faceTexCoords[4], glm::vec4(1.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), faceTexCoords[4], glm::vec4(1.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), faceTexCoords[4], glm::vec4(1.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), faceTexCoords[4], glm::vec4(1.0f)},
        
        // Bottom face
        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), faceTexCoords[5], glm::vec4(1.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), faceTexCoords[5], glm::vec4(1.0f)},
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), faceTexCoords[5], glm::vec4(1.0f)},
        {glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), faceTexCoords[5], glm::vec4(1.0f)}
    };
    
    return mesh;
}

BlockRegistry& BlockRegistry::getInstance() {
    static BlockRegistry sInstance;
    return sInstance;
}