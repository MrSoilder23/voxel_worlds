#include "./systems/chunk_meshing_system.hpp"

// Private
inline const ChunkStorageComponent* ChunkMeshingSystem::getNeighbouringChunk(
    bismuth::Registry & registry,
    entityMap3   const& storageComponents,
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

inline void ChunkMeshingSystem::fillBlockSide(
    MeshComponent const& blockObject,
    MeshComponent      & chunkModel,
    int           const  textureSide,
    int           const  vertexOffset, 
    GLuint        const& baseOffset, 
    glm::vec3     const& chunkOffset,
    GLuint a, 
    GLuint b, 
    GLuint c,
    GLuint d 
) {
    const glm::vec2& pos = blockObject.vertices[textureSide].texCoords;
    
    const glm::vec2 posBegin = pos * (1.0f/6.0f);
    const glm::vec2 posEnd   = posBegin + (1.0f/6.0f);

    MeshComponent tempMesh = blockObject;

    chunkModel.indices.insert(chunkModel.indices.end(), {
        a + baseOffset, b + baseOffset, c + baseOffset,
        a + baseOffset, c + baseOffset, d + baseOffset
    }); 

    auto& vertice1 = tempMesh.vertices[0 + vertexOffset];
    auto& vertice2 = tempMesh.vertices[1 + vertexOffset];
    auto& vertice3 = tempMesh.vertices[2 + vertexOffset];
    auto& vertice4 = tempMesh.vertices[3 + vertexOffset];

    vertice1.position += chunkOffset;
    vertice2.position += chunkOffset;
    vertice3.position += chunkOffset;
    vertice4.position += chunkOffset;

    vertice1.texCoords = glm::vec2(posEnd.x,   posEnd.y),
    vertice2.texCoords = glm::vec2(posBegin.x, posEnd.y),
    vertice3.texCoords = glm::vec2(posEnd.x,   posBegin.y),
    vertice4.texCoords = glm::vec2(posBegin.x, posBegin.y),

    chunkModel.vertices.push_back(std::move(vertice1));
    chunkModel.vertices.push_back(std::move(vertice2));
    chunkModel.vertices.push_back(std::move(vertice3));
    chunkModel.vertices.push_back(std::move(vertice4));
}

inline void ChunkMeshingSystem::rightBoundary(
    ChunkStorageComponent const*& targetChunk,
    int                        & target,
    ChunkStorageComponent const* currentStorage,
    ChunkStorageComponent const* neigbourStorage
) {
    if (target < VoxelWorlds::CHUNK_SIZE) {
        targetChunk = currentStorage;
    } else if (neigbourStorage) {
        targetChunk = neigbourStorage;
        target = 0;
    }
}
inline void ChunkMeshingSystem::leftBoundary(
    ChunkStorageComponent const*& targetChunk,
    int                        & target,
    ChunkStorageComponent const* currentStorage,
    ChunkStorageComponent const* neigbourStorage
) {
    if (target >= 0) {
        targetChunk = currentStorage;
    } else if (neigbourStorage) {
        targetChunk = neigbourStorage;
        target = VoxelWorlds::CHUNK_SIZE-1;
    }
}

inline void ChunkMeshingSystem::getBlockNeighbours(
    ChunkStorageComponent const* storage,
    MeshComponent              & chunkModel,
    BlockTypes            const& block,
    GLuint                const  vertexSize,
    ChunkStorageComponent const* chunkRight,
    ChunkStorageComponent const* chunkLeft,
    ChunkStorageComponent const* chunkTop,
    ChunkStorageComponent const* chunkBot,
    ChunkStorageComponent const* chunkFront,
    ChunkStorageComponent const* chunkBack,
    int                   const  blockX,
    int                   const  blockY,
    int                   const  blockZ
) {
    static BlockRegistry& blockRegistry = BlockRegistry::getInstance();

    const auto& blockObject = blockRegistry.getMesh(block);
    const glm::vec3 chunkOffset = glm::vec3(blockX,blockY,blockZ);

    unsigned int size = 0;
    GLuint baseOffset = size + vertexSize;

    // RightFace
    const ChunkStorageComponent* targetChunk = nullptr;  
    int target = blockX + 1;
    
    rightBoundary(targetChunk, target, storage, chunkRight);
    
    baseOffset = size + vertexSize;
    if (targetChunk && ChunkStorage::getBlock(const_cast<ChunkStorageComponent&>(*targetChunk), target, blockY, blockZ) == BlockTypes::air) {
        fillBlockSide(blockObject, chunkModel, 1, 8, baseOffset, chunkOffset);
        size += 4;
    }

    // LeftFace
    targetChunk = nullptr;
    target = blockX-1;

    leftBoundary(targetChunk, target, storage, chunkLeft);

    baseOffset = size + vertexSize;
    if (targetChunk && ChunkStorage::getBlock(const_cast<ChunkStorageComponent&>(*targetChunk), target, blockY, blockZ) == BlockTypes::air) {
        fillBlockSide(blockObject, chunkModel, 0, 12, baseOffset, chunkOffset);
        size += 4;
    }

    // TopFace
    targetChunk = nullptr;
    target = blockY + 1;

    rightBoundary(targetChunk, target, storage, chunkTop);

    baseOffset = size + vertexSize;
    if (targetChunk && ChunkStorage::getBlock(const_cast<ChunkStorageComponent&>(*targetChunk), blockX, target, blockZ) == BlockTypes::air) {
        fillBlockSide(blockObject, chunkModel, 4, 16, baseOffset, chunkOffset);
        size += 4;
    }

    // BotFace
    targetChunk = nullptr;
    target = blockY-1;

    leftBoundary(targetChunk, target, storage, chunkBot);

    baseOffset = size + vertexSize;
    if (targetChunk && ChunkStorage::getBlock(const_cast<ChunkStorageComponent&>(*targetChunk), target, blockY, blockZ) == BlockTypes::air) {
        fillBlockSide(blockObject, chunkModel, 5, 20, baseOffset, chunkOffset);
        size += 4;
    }

    // FrontFace
    targetChunk = nullptr;
    target = blockZ + 1;

    rightBoundary(targetChunk, target, storage, chunkFront);

    baseOffset = size + vertexSize;
    if (targetChunk && ChunkStorage::getBlock(const_cast<ChunkStorageComponent&>(*targetChunk), blockX, blockY, target) == BlockTypes::air) {
        fillBlockSide(blockObject, chunkModel, 2, 0, baseOffset, chunkOffset);
        size += 4;
    }

    // BackFace
    targetChunk = nullptr;
    target = blockZ-1;

    leftBoundary(targetChunk, target, storage, chunkBack);

    baseOffset = size + vertexSize;
    if (targetChunk && ChunkStorage::getBlock(const_cast<ChunkStorageComponent&>(*targetChunk), blockX, blockY, target) == BlockTypes::air) {
        fillBlockSide(blockObject, chunkModel, 3, 4, baseOffset, chunkOffset, 3,1,0,2);
        size += 4;
    }
}




// To-Do DELETE
inline bool ChunkMeshingSystem::checkBlock(
    bismuth::ComponentPool<ChunkStorageComponent>& chunkStoragePool,
    entityMap3      const& entityMap,
    ChunkStorageComponent& currentChunkData,
    int chunkX,
    int chunkY,
    int chunkZ,
    int x,
    int y,
    int z
) {
    const int originalChunkX = chunkX;
    const int originalChunkY = chunkY;
    const int originalChunkZ = chunkZ;
    
    if(x < 0) {
        x = VoxelWorlds::CHUNK_SIZE-1;
        chunkX--;
    } else if(x > VoxelWorlds::CHUNK_SIZE-1) {
        x = 0;
        chunkX++;
    } else if(y < 0) {
        y = VoxelWorlds::CHUNK_SIZE-1;
        chunkY--;
    } else if(y > VoxelWorlds::CHUNK_SIZE-1) {
        y = 0;
        chunkY++;
    } else if(z < 0) {
        z = VoxelWorlds::CHUNK_SIZE-1;
        chunkZ--;
    } else if(z > VoxelWorlds::CHUNK_SIZE-1) {
        z = 0;
        chunkZ++;
    }
    
    if(originalChunkX != chunkX || originalChunkY != chunkY || originalChunkZ != chunkZ) {
        const auto& it = entityMap.find(glm::ivec3(chunkX, chunkY, chunkZ));

        if(it != entityMap.end()) {
            auto& chunkData = chunkStoragePool.getComponent(it->second);
            return ChunkStorage::getBlock(chunkData, x,y,z) == BlockTypes::air;
        }
        
        return true;
    }

    return ChunkStorage::getBlock(currentChunkData, x,y,z) == BlockTypes::air;
}




// Public
void ChunkMeshingSystem::update(bismuth::Registry& registry) {
    auto chunkView = registry.getView<MeshComponent, MaterialComponent, ChunkStateComponent, ChunkStorageComponent, PositionComponent, BoundingBoxCollectionComponent>();

    static BlockTextureCreator& blockTextures = BlockTextureCreator::getInstance();
    static auto texture = blockTextures.getTexture("ChunkTexture");

    entityMap3 chunkMap;
    for (auto [entity, mesh, material, state, storage, position, boundingCollection] : chunkView) {
        glm::ivec3 chunkCoords = position.position / VoxelWorlds::CHUNK_SIZE;
        chunkMap[chunkCoords] = entity;
    }
    

    for(auto [entity, mesh, material, state, storage, position, boundingCollection] : chunkView) {
        if(state.progress == ChunkProgress::fully_generated) {
            continue;
        }

        const glm::ivec3 chunkPos = position.position / VoxelWorlds::CHUNK_SIZE;

        const auto& chunkRight = getNeighbouringChunk(registry, chunkMap, chunkPos.x+1, chunkPos.y, chunkPos.z);
        const auto& chunkLeft  = getNeighbouringChunk(registry, chunkMap, chunkPos.x-1, chunkPos.y, chunkPos.z);
        const auto& chunkTop   = getNeighbouringChunk(registry, chunkMap, chunkPos.x, chunkPos.y+1, chunkPos.z);
        const auto& chunkBot   = getNeighbouringChunk(registry, chunkMap, chunkPos.x, chunkPos.y-1, chunkPos.z);
        const auto& chunkFront = getNeighbouringChunk(registry, chunkMap, chunkPos.x, chunkPos.y, chunkPos.z+1);
        const auto& chunkBack  = getNeighbouringChunk(registry, chunkMap, chunkPos.x, chunkPos.y, chunkPos.z-1);

        if(state.progress == ChunkProgress::partially_generated) {
            if(!chunkRight || !chunkLeft || !chunkTop || !chunkBot || !chunkFront || !chunkBack) {
                continue;
            }
        }

        BoundingBoxCollectionComponent bBoxCollection;
        MeshComponent chunkModel;
        
        for(int blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
            for(int blockY = 0; blockY < VoxelWorlds::CHUNK_SIZE; blockY++) {
                for(int blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {
                    const BlockTypes& block = ChunkStorage::getBlock(storage, blockX, blockY, blockZ);
                    
                    if(block == BlockTypes::air) {
                        continue;
                    }
                    
                    const GLuint vertexSize = chunkModel.vertices.size();

                    getBlockNeighbours(
                        &storage, 
                        chunkModel,
                        block, 
                        vertexSize,
                        chunkRight, 
                        chunkLeft, 
                        chunkTop, 
                        chunkBot,
                        chunkFront, 
                        chunkBack,
                        blockX, blockY, blockZ
                    );

                    if(vertexSize == chunkModel.vertices.size()) {
                        continue;
                    }
    
                    thread_local BoundingBoxComponent bBox;
                    bBox.worldMin = glm::vec3(
                        -0.5f+blockX+VoxelWorlds::CHUNK_SIZE*chunkPos.x,
                        -0.5f+blockY+VoxelWorlds::CHUNK_SIZE*chunkPos.y,
                        -0.5f+blockZ+VoxelWorlds::CHUNK_SIZE*chunkPos.z
                    );
    
                    bBox.worldMax = glm::vec3(
                         0.5f+blockX+VoxelWorlds::CHUNK_SIZE*chunkPos.x, 
                         0.5f+blockY+VoxelWorlds::CHUNK_SIZE*chunkPos.y, 
                         0.5f+blockZ+VoxelWorlds::CHUNK_SIZE*chunkPos.z
                    );
    
                    bBoxCollection.boundingBoxes.push_back(std::move(bBox));
                }
            }
        }
        
        material.textureAtlas = texture;

        mesh = std::move(chunkModel);
        
        boundingCollection = std::move(bBoxCollection);
        if(state.progress == ChunkProgress::pending) {
            state.progress = ChunkProgress::partially_generated;
        } else {
            state.progress = ChunkProgress::fully_generated;
        }
    }
}