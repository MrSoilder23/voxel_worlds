#include "systems/chunk_meshing.hpp"
#include <iostream>

void ChunkMeshingSystem::update(bismuth::Registry& registry) {
    auto chunkView = registry.getView<MeshComponent, PositionComponent, ChunkStorageComponent, MaterialComponent, BoundingBoxCollectionComponent, ChunkStateComponent>();

    static BlockTextureCreator& blockTextures = BlockTextureCreator::getInstance();
    static auto texture = blockTextures.getTexture("ChunkTexture");

    entityMap3 chunkMap;
    for(auto [entity, mesh, position, storage, material, bBox, state] : chunkView) {
        glm::ivec3 chunkCoords = position.position;
        chunkMap[chunkCoords] = entity;
    }

    for(auto [entity, mesh, position, storage, material, bBox, state] : chunkView) {

        NeighboringChunks neighboringChunks = {
            getStorage(registry, chunkMap, position.position.x, position.position.y, position.position.z),   // Center
            getStorage(registry, chunkMap, position.position.x+1, position.position.y, position.position.z), // Right
            getStorage(registry, chunkMap, position.position.x-1, position.position.y, position.position.z), // Left
            getStorage(registry, chunkMap, position.position.x, position.position.y+1, position.position.z), // Top
            getStorage(registry, chunkMap, position.position.x, position.position.y-1, position.position.z), // Bottom
            getStorage(registry, chunkMap, position.position.x, position.position.y, position.position.z+1), // Front
            getStorage(registry, chunkMap, position.position.x, position.position.y, position.position.z-1)  // Back
        };

        std::vector<uint64_t> bitChunk(VoxelWorlds::CHUNK_SIZE_2D * 3);
        std::vector<uint64_t> faceMask(VoxelWorlds::CHUNK_SIZE_2D * 3 * 2);
        
        for(int z = 0; z < VoxelWorlds::CHUNK_SIZE_PADDING; z++) {
            for(int y = 0; y < VoxelWorlds::CHUNK_SIZE_PADDING; y++) {
                for(int x = 0; x < VoxelWorlds::CHUNK_SIZE_PADDING; x++) {
                    glm::ivec3 blockPos = glm::ivec3(x,y,z) - glm::ivec3(1);
                    bool isBlock = checkBlock(neighboringChunks, blockPos);

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
                            
                        BoundingBoxComponent bBox;
                        bBox.worldMin = glm::vec3(
                            -0.5f + pos.x + position.position.x,
                            -0.5f + pos.y + position.position.y,
                            -0.5f + pos.z + position.position.z
                        );
        
                        bBox.worldMax = glm::vec3(
                            0.5f + pos.x + position.position.x,
                            0.5f + pos.y + position.position.y,
                            0.5f + pos.z + position.position.z
                        );
                        
                        bBoxCollection.boundingBoxes.push_back(std::move(bBox));
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
    bismuth::Registry& registry, 
    entityMap3  const& storageComponents,
    int x,
    int y,
    int z
) {
    auto& storagePool = registry.getComponentPool<ChunkStorageComponent>();
    
    glm::ivec3 coords = {x,y,z};
    
    auto it = storageComponents.find(coords);
    if(it == storageComponents.end()) {
        return nullptr;
    }
    
    return &storagePool.getComponent(it->second);
}

bool ChunkMeshingSystem::checkBlock(
    NeighboringChunks const& chunks,
    glm::ivec3        const& localBlockPos
) {
    int chunkX = 0, chunkY = 0, chunkZ = 0;
    ChunkStorageComponent* target = nullptr;

    if(localBlockPos.x < 0) chunkX = -1;
    else if(localBlockPos.x > VoxelWorlds::CHUNK_SIZE) chunkX = 1;
    if(localBlockPos.y < 0) chunkY = -1;
    else if(localBlockPos.y > VoxelWorlds::CHUNK_SIZE) chunkY = 1;
    if(localBlockPos.z < 0) chunkZ = -1;
    else if(localBlockPos.z > VoxelWorlds::CHUNK_SIZE) chunkZ = 1;

    
    if(((chunkZ != 0) + (chunkY != 0) + (chunkZ != 0)) > 1) {
        return false;
    }
    
    if(chunkX == 0 && chunkY == 0 && chunkZ == 0) {
        target = chunks.center;
    } else if(chunkX == -1) {
        target = chunks.left;
    } else if(chunkX == 1) {
        target = chunks.right;
    } else if(chunkY == -1) {
        target = chunks.bot;
    } else if(chunkY == 1) {
        target = chunks.top;
    } else if(chunkZ == -1) {
        target = chunks.back;
    } else if(chunkZ == 1) {
        target = chunks.front;
    }
    
    if(!target) {
        return true;
    }

    int localX = localBlockPos.x < 0 ? VoxelWorlds::CHUNK_SIZE-1 : (localBlockPos.x >= VoxelWorlds::CHUNK_SIZE ? 0 : localBlockPos.x);
    int localY = localBlockPos.y < 0 ? VoxelWorlds::CHUNK_SIZE-1 : (localBlockPos.y >= VoxelWorlds::CHUNK_SIZE ? 0 : localBlockPos.y);
    int localZ = localBlockPos.z < 0 ? VoxelWorlds::CHUNK_SIZE-1 : (localBlockPos.z >= VoxelWorlds::CHUNK_SIZE ? 0 : localBlockPos.z);

    return ChunkStorage::getBlock(*target, localX, localY, localZ) != BlockTypes::air;
}