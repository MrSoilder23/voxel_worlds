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

void GetBlockNeighbours(
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

    auto& blockObject = blockRegistry.GetBlock(block);
    const glm::vec3 chunkOffset = glm::vec3(blockX,blockY,blockZ);

    unsigned int size = 0;

    // RightFace
    if(blockX + 1 < VoxelWorlds::CHUNK_SIZE) {
        if(ChunkStorage::GetBlock(*storage, blockX+1, blockY, blockZ) == BlockTypes::air){
            chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
                2 + size + vertexSize, 0 + size + vertexSize, 1 + size + vertexSize,
                2 + size + vertexSize, 1 + size + vertexSize, 3 + size + vertexSize
            });
            size += 4;

            chunkModel.mModel.vertexPositions.insert(
                chunkModel.mModel.vertexPositions.end(), {
                blockObject.model.vertexPositions[8] + chunkOffset,
                blockObject.model.vertexPositions[9] + chunkOffset,
                blockObject.model.vertexPositions[10] + chunkOffset,
                blockObject.model.vertexPositions[11] + chunkOffset,
            });
            glm::vec2 pos = blockObject.textureCoords[1];

            glm::vec2 posBegin = pos * (1.0f/6.0f);
            glm::vec2 posEnd = posBegin + (1.0f/6.0f);

            chunkModel.mTexturePositions.insert(
                chunkModel.mTexturePositions.end(), {
                glm::vec2(posEnd.x, posEnd.y),
                glm::vec2(posBegin.x, posEnd.y),
                glm::vec2(posEnd.x, posBegin.y),
                glm::vec2(posBegin.x, posBegin.y),
            });
        }
    } else if (chunkRight) {
        if(ChunkStorage::GetBlock(*chunkRight, 0, blockY, blockZ) == BlockTypes::air) {
            chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
                2 + size + vertexSize, 0 + size + vertexSize, 1 + size + vertexSize,
                2 + size + vertexSize, 1 + size + vertexSize, 3 + size + vertexSize
            });
            size += 4;

            chunkModel.mModel.vertexPositions.insert(
                chunkModel.mModel.vertexPositions.end(), {
                blockObject.model.vertexPositions[8] + chunkOffset,
                blockObject.model.vertexPositions[9] + chunkOffset,
                blockObject.model.vertexPositions[10] + chunkOffset,
                blockObject.model.vertexPositions[11] + chunkOffset,
            });

            glm::vec2 pos = blockObject.textureCoords[1];

            glm::vec2 posBegin = pos * (1.0f/6.0f);
            glm::vec2 posEnd = posBegin + (1.0f/6.0f);

            chunkModel.mTexturePositions.insert(
                chunkModel.mTexturePositions.end(), {
                glm::vec2(posEnd.x, posEnd.y),
                glm::vec2(posBegin.x, posEnd.y),
                glm::vec2(posEnd.x, posBegin.y),
                glm::vec2(posBegin.x, posBegin.y),
            });
        }
    }

    // LeftFace
    if(blockX - 1 >= 0) {
        if(ChunkStorage::GetBlock(*storage, blockX-1, blockY, blockZ) == BlockTypes::air){
            chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
                2 + size + vertexSize, 0 + size + vertexSize, 1 + size + vertexSize,
                2 + size + vertexSize, 1 + size + vertexSize, 3 + size + vertexSize
            });
            size += 4;

            chunkModel.mModel.vertexPositions.insert(
                chunkModel.mModel.vertexPositions.end(), {
                blockObject.model.vertexPositions[12] + chunkOffset,
                blockObject.model.vertexPositions[13] + chunkOffset,
                blockObject.model.vertexPositions[14] + chunkOffset,
                blockObject.model.vertexPositions[15] + chunkOffset,
            });

            glm::vec2 pos = blockObject.textureCoords[0];

            glm::vec2 posBegin = pos * (1.0f/6.0f);
            glm::vec2 posEnd = posBegin + (1.0f/6.0f);

            chunkModel.mTexturePositions.insert(
                chunkModel.mTexturePositions.end(), {
                glm::vec2(posEnd.x, posEnd.y),
                glm::vec2(posBegin.x, posEnd.y),
                glm::vec2(posEnd.x, posBegin.y),
                glm::vec2(posBegin.x, posBegin.y),
            });
        }
    } else if (chunkLeft) {
        if(ChunkStorage::GetBlock(*chunkLeft, VoxelWorlds::CHUNK_SIZE-1, blockY, blockZ) == BlockTypes::air) {
            chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
                2 + size + vertexSize, 0 + size + vertexSize, 1 + size + vertexSize,
                2 + size + vertexSize, 1 + size + vertexSize, 3 + size + vertexSize
            });
            size += 4;

            chunkModel.mModel.vertexPositions.insert(
                chunkModel.mModel.vertexPositions.end(), {
                blockObject.model.vertexPositions[12] + chunkOffset,
                blockObject.model.vertexPositions[13] + chunkOffset,
                blockObject.model.vertexPositions[14] + chunkOffset,
                blockObject.model.vertexPositions[15] + chunkOffset,
            });

            glm::vec2 pos = blockObject.textureCoords[0];

            glm::vec2 posBegin = pos * (1.0f/6.0f);
            glm::vec2 posEnd = posBegin + (1.0f/6.0f);

            chunkModel.mTexturePositions.insert(
                chunkModel.mTexturePositions.end(), {
                glm::vec2(posEnd.x, posEnd.y),
                glm::vec2(posBegin.x, posEnd.y),
                glm::vec2(posEnd.x, posBegin.y),
                glm::vec2(posBegin.x, posBegin.y),
            });
        }
    }

    // TopFace
    if(blockY + 1 < VoxelWorlds::CHUNK_SIZE) {
        if(ChunkStorage::GetBlock(*storage, blockX, blockY+1, blockZ) == BlockTypes::air){
            chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
                2 + size + vertexSize, 0 + size + vertexSize, 1 + size + vertexSize,
                2 + size + vertexSize, 1 + size + vertexSize, 3 + size + vertexSize
            });
            size += 4;

            chunkModel.mModel.vertexPositions.insert(
                chunkModel.mModel.vertexPositions.end(), {
                blockObject.model.vertexPositions[16] + chunkOffset,
                blockObject.model.vertexPositions[17] + chunkOffset,
                blockObject.model.vertexPositions[18] + chunkOffset,
                blockObject.model.vertexPositions[19] + chunkOffset,
            });

            glm::vec2 pos = blockObject.textureCoords[4];

            glm::vec2 posBegin = pos * (1.0f/6.0f);
            glm::vec2 posEnd = posBegin + (1.0f/6.0f);

            chunkModel.mTexturePositions.insert(
                chunkModel.mTexturePositions.end(), {
                glm::vec2(posEnd.x, posEnd.y),
                glm::vec2(posBegin.x, posEnd.y),
                glm::vec2(posEnd.x, posBegin.y),
                glm::vec2(posBegin.x, posBegin.y),
            });
        }
    } else if (chunkTop) {
        if(ChunkStorage::GetBlock(*chunkTop, blockX, 0, blockZ) == BlockTypes::air) {
            chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
                2 + size + vertexSize, 0 + size + vertexSize, 1 + size + vertexSize,
                2 + size + vertexSize, 1 + size + vertexSize, 3 + size + vertexSize
            });
            size += 4;

            chunkModel.mModel.vertexPositions.insert(
                chunkModel.mModel.vertexPositions.end(), {
                blockObject.model.vertexPositions[16] + chunkOffset,
                blockObject.model.vertexPositions[17] + chunkOffset,
                blockObject.model.vertexPositions[18] + chunkOffset,
                blockObject.model.vertexPositions[19] + chunkOffset,
            });
            
            glm::vec2 pos = blockObject.textureCoords[4];

            glm::vec2 posBegin = pos * (1.0f/6.0f);
            glm::vec2 posEnd = posBegin + (1.0f/6.0f);

            chunkModel.mTexturePositions.insert(
                chunkModel.mTexturePositions.end(), {
                glm::vec2(posEnd.x, posEnd.y),
                glm::vec2(posBegin.x, posEnd.y),
                glm::vec2(posEnd.x, posBegin.y),
                glm::vec2(posBegin.x, posBegin.y),
            });
        }
    }

    // BotFace
    if(blockY - 1 >= 0) {
        if(ChunkStorage::GetBlock(*storage, blockX, blockY-1, blockZ) == BlockTypes::air){
            chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
                2 + size + vertexSize, 0 + size + vertexSize, 1 + size + vertexSize,
                2 + size + vertexSize, 1 + size + vertexSize, 3 + size + vertexSize
            });
            size += 4;

            chunkModel.mModel.vertexPositions.insert(
                chunkModel.mModel.vertexPositions.end(), {
                blockObject.model.vertexPositions[20] + chunkOffset,
                blockObject.model.vertexPositions[21] + chunkOffset,
                blockObject.model.vertexPositions[22] + chunkOffset,
                blockObject.model.vertexPositions[23] + chunkOffset,
            });

            glm::vec2 pos = blockObject.textureCoords[5];

            glm::vec2 posBegin = pos * (1.0f/6.0f);
            glm::vec2 posEnd = posBegin + (1.0f/6.0f);

            chunkModel.mTexturePositions.insert(
                chunkModel.mTexturePositions.end(), {
                glm::vec2(posEnd.x, posEnd.y),
                glm::vec2(posBegin.x, posEnd.y),
                glm::vec2(posEnd.x, posBegin.y),
                glm::vec2(posBegin.x, posBegin.y),
            });
        }
    } else if (chunkBot) {
        if(ChunkStorage::GetBlock(*chunkBot, blockX, VoxelWorlds::CHUNK_SIZE-1, blockZ) == BlockTypes::air) {
            chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
                2 + size + vertexSize, 0 + size + vertexSize, 1 + size + vertexSize,
                2 + size + vertexSize, 1 + size + vertexSize, 3 + size + vertexSize
            });
            size += 4;

            chunkModel.mModel.vertexPositions.insert(
                chunkModel.mModel.vertexPositions.end(), {
                blockObject.model.vertexPositions[20] + chunkOffset,
                blockObject.model.vertexPositions[21] + chunkOffset,
                blockObject.model.vertexPositions[22] + chunkOffset,
                blockObject.model.vertexPositions[23] + chunkOffset,
            });

            glm::vec2 pos = blockObject.textureCoords[5];

            glm::vec2 posBegin = pos * (1.0f/6.0f);
            glm::vec2 posEnd = posBegin + (1.0f/6.0f);

            chunkModel.mTexturePositions.insert(
                chunkModel.mTexturePositions.end(), {
                glm::vec2(posEnd.x, posEnd.y),
                glm::vec2(posBegin.x, posEnd.y),
                glm::vec2(posEnd.x, posBegin.y),
                glm::vec2(posBegin.x, posBegin.y),
            });
        }
    }

    // FrontFace
    if(blockZ + 1 < VoxelWorlds::CHUNK_SIZE) {
        if(ChunkStorage::GetBlock(*storage, blockX, blockY, blockZ+1) == BlockTypes::air){
            chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
                2 + size + vertexSize, 0 + size + vertexSize, 1 + size + vertexSize,
                2 + size + vertexSize, 1 + size + vertexSize, 3 + size + vertexSize
            }); 
            size += 4;

            chunkModel.mModel.vertexPositions.insert(
                chunkModel.mModel.vertexPositions.end(), {
                blockObject.model.vertexPositions[0] + chunkOffset,
                blockObject.model.vertexPositions[1] + chunkOffset,
                blockObject.model.vertexPositions[2] + chunkOffset,
                blockObject.model.vertexPositions[3] + chunkOffset,
            });

            glm::vec2 pos = blockObject.textureCoords[2];

            glm::vec2 posBegin = pos * (1.0f/6.0f);
            glm::vec2 posEnd = posBegin + (1.0f/6.0f);

            chunkModel.mTexturePositions.insert(
                chunkModel.mTexturePositions.end(), {
                glm::vec2(posEnd.x, posEnd.y),
                glm::vec2(posBegin.x, posEnd.y),
                glm::vec2(posEnd.x, posBegin.y),
                glm::vec2(posBegin.x, posBegin.y),
            });
        }
    } else if (chunkFront) {
        if(ChunkStorage::GetBlock(*chunkFront, blockX, blockY, 0) == BlockTypes::air) {
            chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
                2 + size + vertexSize, 0 + size + vertexSize, 1 + size + vertexSize,
                2 + size + vertexSize, 1 + size + vertexSize, 3 + size + vertexSize
            });
            size += 4;
            
            chunkModel.mModel.vertexPositions.insert(
                chunkModel.mModel.vertexPositions.end(), {
                blockObject.model.vertexPositions[0] + chunkOffset,
                blockObject.model.vertexPositions[1] + chunkOffset,
                blockObject.model.vertexPositions[2] + chunkOffset,
                blockObject.model.vertexPositions[3] + chunkOffset,
            });

            glm::vec2 pos = blockObject.textureCoords[2];

            glm::vec2 posBegin = pos * (1.0f/6.0f);
            glm::vec2 posEnd = posBegin + (1.0f/6.0f);

            chunkModel.mTexturePositions.insert(
                chunkModel.mTexturePositions.end(), {
                glm::vec2(posEnd.x, posEnd.y),
                glm::vec2(posBegin.x, posEnd.y),
                glm::vec2(posEnd.x, posBegin.y),
                glm::vec2(posBegin.x, posBegin.y),
            });
        }
    }

    // BackFace
    if(blockZ - 1 >= 0) {
        if(ChunkStorage::GetBlock(*storage, blockX, blockY, blockZ-1) == BlockTypes::air){
            chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
                3 + size + vertexSize, 1 + size + vertexSize, 0 + size + vertexSize,
                3 + size + vertexSize, 0 + size + vertexSize, 2 + size + vertexSize
            });
            size += 4;

            chunkModel.mModel.vertexPositions.insert(
                chunkModel.mModel.vertexPositions.end(), {
                blockObject.model.vertexPositions[4] + chunkOffset,
                blockObject.model.vertexPositions[5] + chunkOffset,
                blockObject.model.vertexPositions[6] + chunkOffset,
                blockObject.model.vertexPositions[7] + chunkOffset,
            });

            glm::vec2 pos = blockObject.textureCoords[3];

            glm::vec2 posBegin = pos * (1.0f/6.0f);
            glm::vec2 posEnd = posBegin + (1.0f/6.0f);

            chunkModel.mTexturePositions.insert(
                chunkModel.mTexturePositions.end(), {
                glm::vec2(posEnd.x, posEnd.y),
                glm::vec2(posBegin.x, posEnd.y),
                glm::vec2(posEnd.x, posBegin.y),
                glm::vec2(posBegin.x, posBegin.y),
            });
        }
    } else if (chunkBack) {
        if(ChunkStorage::GetBlock(*chunkBack, blockX, blockY, VoxelWorlds::CHUNK_SIZE-1) == BlockTypes::air) {
            chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), {
                3 + size + vertexSize, 1 + size + vertexSize, 0 + size + vertexSize,
                3 + size + vertexSize, 0 + size + vertexSize, 2 + size + vertexSize
            });
            size += 4;

            chunkModel.mModel.vertexPositions.insert(
                chunkModel.mModel.vertexPositions.end(), {
                blockObject.model.vertexPositions[4] + chunkOffset,
                blockObject.model.vertexPositions[5] + chunkOffset,
                blockObject.model.vertexPositions[6] + chunkOffset,
                blockObject.model.vertexPositions[7] + chunkOffset,
            });

            glm::vec2 pos = blockObject.textureCoords[3];

            glm::vec2 posBegin = pos * (1.0f/6.0f);
            glm::vec2 posEnd = posBegin + (1.0f/6.0f);

            chunkModel.mTexturePositions.insert(
                chunkModel.mTexturePositions.end(), {
                glm::vec2(posEnd.x, posEnd.y),
                glm::vec2(posBegin.x, posEnd.y),
                glm::vec2(posEnd.x, posBegin.y),
                glm::vec2(posBegin.x, posBegin.y),
            });
        }
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