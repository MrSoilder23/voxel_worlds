#include "./systems/chunk_meshing_system.hpp"

inline ChunkStorageComponent* ChunkMeshingSystem::GetNeighbouringChunk(
    EntityManager& entityManager, const tbb::concurrent_hash_map<glm::ivec3, size_t, Vec3Hash>& entityMap, 
    const std::vector<ChunkStorageComponent*>& storageComponents, int x, int y, int z
) {
    glm::ivec3 coords = {x,y,z};

    chunkEntityIDs::const_accessor chunkEntityAccessor;
    if(!entityMap.find(chunkEntityAccessor, coords)) {
        return nullptr;
    }

    return storageComponents[chunkEntityAccessor->second];
}

inline void FillBlockSide(
    const BlockTemplate& blockObject,
    ChunkModelComponent& chunkModel,
    const int textureSide,
    const int vertexOffset, const GLuint& baseOffset, 
    const glm::vec3& chunkOffset,
    GLuint a = 2, GLuint b = 0, GLuint c = 1, GLuint d = 3 
) {
            
    chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
        a + baseOffset, b + baseOffset, c + baseOffset,
        a + baseOffset, c + baseOffset, d + baseOffset
    }); 

    chunkModel.mModel.vertexPositions.insert(
        chunkModel.mModel.vertexPositions.end(), {
        blockObject.model.vertexPositions[0 + vertexOffset] + chunkOffset,
        blockObject.model.vertexPositions[1 + vertexOffset] + chunkOffset,
        blockObject.model.vertexPositions[2 + vertexOffset] + chunkOffset,
        blockObject.model.vertexPositions[3 + vertexOffset] + chunkOffset,
    });

    const glm::vec2& pos = blockObject.textureCoords[textureSide];

    const glm::vec2 posBegin = pos * (1.0f/6.0f);
    const glm::vec2 posEnd = posBegin + (1.0f/6.0f);

    chunkModel.mTexturePositions.insert(
        chunkModel.mTexturePositions.end(), {
        glm::vec2(posEnd.x, posEnd.y),
        glm::vec2(posBegin.x, posEnd.y),
        glm::vec2(posEnd.x, posBegin.y),
        glm::vec2(posBegin.x, posBegin.y),
    });
}

inline void RightBoundary(ChunkStorageComponent*& targetChunk, int& target, ChunkStorageComponent* currentStorage, ChunkStorageComponent* neigbourStorage) {
    if (target < VoxelWorlds::CHUNK_SIZE) {
        targetChunk = currentStorage;
    } else if (neigbourStorage) {
        targetChunk = neigbourStorage;
        target = 0;
    }
}
inline void LeftBoundary(ChunkStorageComponent*& targetChunk, int& target, ChunkStorageComponent* currentStorage, ChunkStorageComponent* neigbourStorage) {
    if (target >= 0) {
        targetChunk = currentStorage;
    } else if (neigbourStorage) {
        targetChunk = neigbourStorage;
        target = VoxelWorlds::CHUNK_SIZE-1;
    }
}

inline void GetBlockNeighbours(
    ChunkModelComponent& chunkModel,
    ChunkStorageComponent* storage,
    const BlockTypes& block,
    const GLuint vertexSize,
    const auto& chunkRight, const auto& chunkLeft, 
    const auto& chunkTop, const auto& chunkBot,  
    const auto& chunkFront, const auto& chunkBack,
    const int blockX, const int blockY, const int blockZ
) {
    static BlockRegistry& blockRegistry = BlockRegistry::GetInstance();

    const auto& blockObject = blockRegistry.GetBlock(block);
    const glm::vec3 chunkOffset = glm::vec3(blockX,blockY,blockZ);

    unsigned int size = 0;
    GLuint baseOffset = size + vertexSize;

    // RightFace
    ChunkStorageComponent* targetChunk = nullptr;  
    int target = blockX + 1;
    
    RightBoundary(targetChunk, target, storage, chunkRight);
    
    baseOffset = size + vertexSize;
    if (targetChunk && ChunkStorage::GetBlock(*targetChunk, target, blockY, blockZ) == BlockTypes::air) {
        FillBlockSide(blockObject, chunkModel, 1, 8, baseOffset, chunkOffset);
        size += 4;
    }

    // LeftFace
    targetChunk = nullptr;
    target = blockX-1;

    LeftBoundary(targetChunk, target, storage, chunkLeft);

    baseOffset = size + vertexSize;
    if (targetChunk && ChunkStorage::GetBlock(*targetChunk, target, blockY, blockZ) == BlockTypes::air) {
        FillBlockSide(blockObject, chunkModel, 0, 12, baseOffset, chunkOffset);
        size += 4;
    }

    // TopFace
    targetChunk = nullptr;
    target = blockY + 1;

    RightBoundary(targetChunk, target, storage, chunkTop);

    baseOffset = size + vertexSize;
    if (targetChunk && ChunkStorage::GetBlock(*targetChunk, blockX, target, blockZ) == BlockTypes::air) {
        FillBlockSide(blockObject, chunkModel, 4, 16, baseOffset, chunkOffset);
        size += 4;
    }

    // BotFace
    targetChunk = nullptr;
    target = blockY-1;

    LeftBoundary(targetChunk, target, storage, chunkBot);

    baseOffset = size + vertexSize;
    if (targetChunk && ChunkStorage::GetBlock(*targetChunk, target, blockY, blockZ) == BlockTypes::air) {
        FillBlockSide(blockObject, chunkModel, 5, 20, baseOffset, chunkOffset);
        size += 4;
    }

    // FrontFace
    targetChunk = nullptr;
    target = blockZ + 1;

    RightBoundary(targetChunk, target, storage, chunkFront);

    baseOffset = size + vertexSize;
    if (targetChunk && ChunkStorage::GetBlock(*targetChunk, blockX, blockY, target) == BlockTypes::air) {
        FillBlockSide(blockObject, chunkModel, 2, 0, baseOffset, chunkOffset);
        size += 4;
    }

    // BackFace
    targetChunk = nullptr;
    target = blockZ-1;

    LeftBoundary(targetChunk, target, storage, chunkBack);

    baseOffset = size + vertexSize;
    if (targetChunk && ChunkStorage::GetBlock(*targetChunk, blockX, blockY, target) == BlockTypes::air) {
        FillBlockSide(blockObject, chunkModel, 3, 4, baseOffset, chunkOffset, 3,1,0,2);
        size += 4;
    }
}

void ChunkMeshingSystem::CreateChunksMesh(EntityManager& entityManager) {
    const auto models = entityManager.GetComponentArray<ChunkModelComponent>();
    const auto states = entityManager.GetComponentArray<ChunkStateComponent>();
    const auto storages = entityManager.GetComponentArray<ChunkStorageComponent>();
    const auto positions = entityManager.GetComponentArray<PositionComponent>();
    const auto boundingCollections = entityManager.GetComponentArray<BoundingBoxCollectionComponent>();
    
    const auto& entities = entityManager.GetChunkEntities();
    const int& size = models.size();

    static BlockTextureCreator& blockTextures = BlockTextureCreator::GetInstance();
    static GLuint textureID = blockTextures.GetTexture("ChunkTexture");

    static tbb::task_arena arena;
    
    arena.execute([&](){

    tbb::parallel_for(0, size, [&](const auto& entityID){
        if(entityID >= models.size()) {
            return;
        }

        auto& model = models[entityID];
        auto& state = states[entityID];
        auto& storage = storages[entityID];
        auto& position = positions[entityID];
        auto& boundingCollection = boundingCollections[entityID];

        if(!model || !state || !storage || !position || !boundingCollection) {
            return;
        }

        if(state->mProgress == ChunkProgress::fully_generated) {
            return;
        }

        const glm::vec3 chunkPos = position->mPosition / VoxelWorlds::CHUNK_SIZE;
        
        const auto& chunkRight = GetNeighbouringChunk(entityManager, entities, storages, chunkPos.x+1, chunkPos.y, chunkPos.z);
        const auto& chunkLeft  = GetNeighbouringChunk(entityManager, entities, storages, chunkPos.x-1, chunkPos.y, chunkPos.z);
        const auto& chunkTop   = GetNeighbouringChunk(entityManager, entities, storages, chunkPos.x, chunkPos.y+1, chunkPos.z);
        const auto& chunkBot   = GetNeighbouringChunk(entityManager, entities, storages, chunkPos.x, chunkPos.y-1, chunkPos.z);
        const auto& chunkFront = GetNeighbouringChunk(entityManager, entities, storages, chunkPos.x, chunkPos.y, chunkPos.z+1);
        const auto& chunkBack  = GetNeighbouringChunk(entityManager, entities, storages, chunkPos.x, chunkPos.y, chunkPos.z-1);

        if(state->mProgress == ChunkProgress::partially_generated) {
            if(!chunkRight || !chunkLeft || !chunkTop || !chunkBot || !chunkFront || !chunkBack) {
                return;
            }
        }

        BoundingBoxCollectionComponent bBoxCollection;
        ChunkModelComponent chunkModel;
        
        for(int blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
            for(int blockY = 0; blockY < VoxelWorlds::CHUNK_SIZE; blockY++) {
                for(int blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {
                    const BlockTypes& block = ChunkStorage::GetBlock(*storage, blockX, blockY, blockZ);
                    
                    if(block == BlockTypes::air) {
                        continue;
                    }
                    
                    const GLuint vertexSize = chunkModel.mModel.vertexPositions.size();

                    GetBlockNeighbours(
                        chunkModel,
                        storage, block, vertexSize,
                        chunkRight, chunkLeft, 
                        chunkTop, chunkBot,
                        chunkFront, chunkBack,
                        blockX, blockY, blockZ
                    );

                    if(vertexSize == chunkModel.mModel.vertexPositions.size()) {
                        continue;
                    }
    
                    thread_local BoundingBoxComponent bBox;
                    bBox.mWorldMin = glm::vec3(
                        -0.5f+blockX+VoxelWorlds::CHUNK_SIZE*chunkPos.x,
                        -0.5f+blockY+VoxelWorlds::CHUNK_SIZE*chunkPos.y,
                        -0.5f+blockZ+VoxelWorlds::CHUNK_SIZE*chunkPos.z
                    );
    
                    bBox.mWorldMax = glm::vec3(
                         0.5f+blockX+VoxelWorlds::CHUNK_SIZE*chunkPos.x, 
                         0.5f+blockY+VoxelWorlds::CHUNK_SIZE*chunkPos.y, 
                         0.5f+blockZ+VoxelWorlds::CHUNK_SIZE*chunkPos.z
                    );
    
                    bBoxCollection.boundingBoxes.push_back(std::move(bBox));
                }
            }
        }
        
        chunkModel.mTexture = textureID;
        chunkModel.mGenerated = false;

        *model = std::move(chunkModel);
        
        *boundingCollection = std::move(bBoxCollection);
        if(state->mProgress == ChunkProgress::pending) {
            state->mProgress = ChunkProgress::partially_generated;
        } else {
            state->mProgress = ChunkProgress::fully_generated;
        }
    });

    });
}

// Private
inline bool ChunkMeshingSystem::CheckBlock(EntityManager& entityManager, ChunkStorageComponent& currentChunkData, int chunkX, int chunkY, int chunkZ, int x, int y, int z) {
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
        char chunkName[32];
        utility::FastChunkName(chunkName, chunkX, chunkY, chunkZ);

        if(auto chunkData = entityManager.GetComponent<ChunkStorageComponent>(chunkName)) {
            return ChunkStorage::GetBlock(*chunkData, x,y,z) == BlockTypes::air;
        }
        
        return true;
    }

    return ChunkStorage::GetBlock(currentChunkData, x,y,z) == BlockTypes::air;
}