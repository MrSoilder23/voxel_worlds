#include "systems/chunk_meshing.hpp"
#include <iostream>

void ChunkMeshingSystem::update(bismuth::Registry& registry) {
    auto chunkView = registry.getView<MeshComponent, PositionComponent, ChunkStorageComponent, MaterialComponent, BoundingBoxCollectionComponent, ChunkStateComponent>();

    static BlockTextureCreator& blockTextures = BlockTextureCreator::getInstance();
    static auto texture = blockTextures.getTexture("ChunkTexture");

    entityMap3 chunkMap;
    for(auto [entity, mesh, position, storage, material, bBox, state] : chunkView) {
        glm::ivec3 chunkCoords = position.position / VoxelWorlds::CHUNK_SIZE;
        chunkMap[chunkCoords] = &storage;
    }

    std::array<uint64_t, VoxelWorlds::CHUNK_SIZE_2D * 3> bitChunk;
    std::array<uint64_t, VoxelWorlds::CHUNK_SIZE_2D * 3 * 2> faceMask;

    for(auto [entity, mesh, position, storage, material, bBox, state] : chunkView) {

        NeighboringChunks neighboringChunks = {
            &storage, // Center
            getStorage(chunkMap, position.position.x+1, position.position.y, position.position.z), // Right
            getStorage(chunkMap, position.position.x-1, position.position.y, position.position.z), // Left
            getStorage(chunkMap, position.position.x, position.position.y+1, position.position.z), // Top
            getStorage(chunkMap, position.position.x, position.position.y-1, position.position.z), // Bottom
            getStorage(chunkMap, position.position.x, position.position.y, position.position.z+1), // Front
            getStorage(chunkMap, position.position.x, position.position.y, position.position.z-1)  // Back
        };

        bitChunk.fill(0);
        faceMask.fill(0);
        
        for(int z = 0; z < VoxelWorlds::CHUNK_SIZE_PADDING; z++) {
            for(int y = 0; y < VoxelWorlds::CHUNK_SIZE_PADDING; y++) {
                for(int x = 0; x < VoxelWorlds::CHUNK_SIZE_PADDING; x++) {
                    int posX = x - 1;
                    int posY = y - 1;
                    int posZ = z - 1;
                    
                    bool isBlock = checkBlock(neighboringChunks, posX, posY, posZ);

                    if (isBlock) {
                        if((z < 32) && (y < 32) && (z > 0) && (y > 0)) {
                            bitChunk[z + (y * VoxelWorlds::CHUNK_SIZE) + VoxelWorlds::CHUNK_SIZE_2D]   |= (uint64_t(1) << uint64_t(x)); // X Left Right
                        }
                        if((x < 32) && (z < 32) && (x > 0) && (z > 0)) {
                            bitChunk[x + (z * VoxelWorlds::CHUNK_SIZE)]                                |= (uint64_t(1) << uint64_t(y)); // Y
                        }
                        if((x < 32) && (y < 32) && (x > 0) && (y > 0)) {
                            bitChunk[x + (y * VoxelWorlds::CHUNK_SIZE) + VoxelWorlds::CHUNK_SIZE_2D*2] |= (uint64_t(1) << uint64_t(z)); // Z
                        }
                    }
                }
            }
        }

        for(int axis = 0; axis < 3; axis++) {
            for(int i = 0; i < VoxelWorlds::CHUNK_SIZE_2D; i++) {
                uint64_t column = bitChunk[(VoxelWorlds::CHUNK_SIZE_2D * axis) + i];

                faceMask[(VoxelWorlds::CHUNK_SIZE_2D * (axis * 2)) + i]     = column & ~(column << 1); // Right faces
                faceMask[(VoxelWorlds::CHUNK_SIZE_2D * (axis * 2 + 1)) + i] = column & ~(column >> 1); // Left faces
            }
        }
        
        BoundingBoxCollectionComponent bBoxCollection;

        for(int axis = 0; axis < 6; axis++) {
            for(int z = 0; z < VoxelWorlds::CHUNK_SIZE; z++) {
                for(int x = 0; x < VoxelWorlds::CHUNK_SIZE; x++) {
                    int columnIndex = x + (z * VoxelWorlds::CHUNK_SIZE) + (VoxelWorlds::CHUNK_SIZE_2D * axis);

                    uint64_t column = faceMask[columnIndex] >> 1;                // Delete right padding
                    column = column & ~(1 << uint64_t(VoxelWorlds::CHUNK_SIZE-1)); // Delete left padding

                    while(column != 0) {
                        int y = std::countr_zero(column);
                        column &= column - 1;

                        glm::ivec3 pos;
                        switch(axis) {
                            case 0:
                            case 1:
                                pos = glm::ivec3(x,y,z);
                                break;
                            case 2:
                            case 3:
                                pos = glm::ivec3(y,z,x);
                                break;
                            default:
                                pos = glm::ivec3(x,z,y);
                                break;
                            }

                        auto& currentBlock = ChunkStorage::getBlock(*neighboringChunks.center, pos.x, pos.y, pos.z);
                        
                        addFace(mesh, currentBlock, pos, axis);
                            
                        BoundingBoxComponent boundingBox;
                        boundingBox.worldMin = glm::vec3(
                            -0.5f + pos.x + position.position.x,
                            -0.5f + pos.y + position.position.y,
                            -0.5f + pos.z + position.position.z
                        );
        
                        boundingBox.worldMax = glm::vec3(
                            0.5f + pos.x + position.position.x,
                            0.5f + pos.y + position.position.y,
                            0.5f + pos.z + position.position.z
                        );
                        
                        bBoxCollection.boundingBoxes.push_back(std::move(boundingBox));
                    }
                }
            }
        }
        material.textureAtlas = texture;

        bBox = std::move(bBoxCollection);
        state.progress = ChunkProgress::fully_generated;
        
    }
}

inline void ChunkMeshingSystem::addFace(
    MeshComponent  & chunkModel,
    BlockTypes     & blockType,
    glm::ivec3 const blockPos,
    int axis
) {
    struct FaceInfo {
        int textureIndex;              // Which texture in the atlas (0-5)
        int vertexOffset;              // Starting vertex index in the block mesh
        std::array<GLuint, 6> indices; // Two triangles (6 indices)
    };
    
    static std::array<FaceInfo, 6> FACE_INFOS = {
        // Right, Left, Front, Back, Top, Bottom faces
        FaceInfo{2, 8,  {0, 1, 2, 0, 2, 3}},  // Axis 0: Right (texture 2, vertices 8-11)
        FaceInfo{3, 12, {0, 1, 2, 0, 2, 3}},  // Axis 1: Left (texture 3, vertices 12-15)
        FaceInfo{0, 0,  {0, 1, 2, 0, 2, 3}},  // Axis 2: Front (texture 0, vertices 0-3)
        FaceInfo{1, 4,  {0, 1, 2, 0, 2, 3}},  // Axis 3: Back (texture 1, vertices 4-7)
        FaceInfo{4, 16, {0, 1, 2, 0, 2, 3}},  // Axis 4: Top (texture 4, vertices 16-19)
        FaceInfo{5, 20, {0, 1, 2, 0, 2, 3}}   // Axis 5: Bottom (texture 5, vertices 20-23)
    };

    if(blockType == BlockTypes::air) {
        return;
    }

    static BlockRegistry& blockRegistry = BlockRegistry::getInstance();
    const auto& blockMesh = blockRegistry.getMesh(blockType);
    const auto& face = FACE_INFOS[axis];

    GLuint baseVertex = chunkModel.vertices.size();

    for(int i = 0; i < 4; i++) {
        Vertex vertex = blockMesh.vertices[face.vertexOffset + i];
        vertex.position += glm::vec3(blockPos);

        const glm::vec2& texCoord = blockMesh.vertices[face.textureIndex].texCoords;
        const glm::vec2 texBegin  = texCoord * (1.0f / 6.0f);
        const glm::vec2 texEnd    = texBegin + (1.0f / 6.0f);

        switch(i) {
            case 0: vertex.texCoords = glm::vec2(texEnd.x, texEnd.y); break;     // Top-right
            case 1: vertex.texCoords = glm::vec2(texBegin.x, texEnd.y); break;   // Top-left
            case 2: vertex.texCoords = glm::vec2(texEnd.x, texBegin.y); break;   // Bottom-right
            case 3: vertex.texCoords = glm::vec2(texBegin.x, texBegin.y); break; // Bottom-left
        }

        chunkModel.vertices.push_back(vertex);
    }

    for(GLuint index : face.indices) {
        chunkModel.indices.push_back(baseVertex + index);
    }
}

inline ChunkStorageComponent* ChunkMeshingSystem::getStorage(
    entityMap3  const& storageComponents,
    int x,
    int y,
    int z
) {    
    glm::ivec3 coords = {x,y,z};
    coords = coords / static_cast<int>(VoxelWorlds::CHUNK_SIZE);
    
    auto it = storageComponents.find(coords);
    if(it == storageComponents.end()) {
        return nullptr;
    }
    
    return it->second;
}

inline bool ChunkMeshingSystem::checkBlock(
    NeighboringChunks const& chunks,
    int               const& localPosX,
    int               const& localPosY,
    int               const& localPosZ
) {
    ChunkStorageComponent* target = nullptr;
    int chunkX = (localPosX >= VoxelWorlds::CHUNK_SIZE) - (localPosX < 0);
    int chunkY = (localPosY >= VoxelWorlds::CHUNK_SIZE) - (localPosY < 0);
    int chunkZ = (localPosZ >= VoxelWorlds::CHUNK_SIZE) - (localPosZ < 0);    
    
    if((chunkX && chunkY) || (chunkX && chunkZ) || (chunkY && chunkZ)) {
        return false;
    }
    
    if(chunkX == 0 && chunkY == 0 && chunkZ == 0) target = chunks.center;
    else if(chunkX == -1) target = chunks.left;
    else if(chunkX == 1)  target = chunks.right;
    else if(chunkY == -1) target = chunks.bot;
    else if(chunkY == 1)  target = chunks.top;
    else if(chunkZ == -1) target = chunks.back;
    else if(chunkZ == 1)  target = chunks.front;
        
    if(!target) {
        return true;
    }

    constexpr int CHUNK_MASK = VoxelWorlds::CHUNK_SIZE - 1;

    int localX = localPosX & CHUNK_MASK;
    int localY = localPosY & CHUNK_MASK;
    int localZ = localPosZ & CHUNK_MASK;

    return ChunkStorage::getBlock(*target, localX, localY, localZ) != BlockTypes::air;
}