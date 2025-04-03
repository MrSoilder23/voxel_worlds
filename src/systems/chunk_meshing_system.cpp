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

void ChunkMeshingSystem::CreateChunksMesh(EntityManager& entityManager) {
    const auto models = entityManager.GetComponentArray<ChunkModelComponent>();
    const auto states = entityManager.GetComponentArray<ChunkStateComponent>();
    const auto storages = entityManager.GetComponentArray<ChunkStorageComponent>();
    const auto positions = entityManager.GetComponentArray<PositionComponent>();
    const auto boundingCollections = entityManager.GetComponentArray<BoundingBoxCollectionComponent>();
    
    static BlockRegistry& blockRegistry = BlockRegistry::GetInstance();
    static std::unordered_map<GLuint64, GLuint> textures = TextureMap();

    const auto& entities = entityManager.GetChunkEntities();
    static tbb::task_arena arena;

    arena.execute([&](){
    const int size = models.size();

    tbb::parallel_for(0, size,  [&](int entityID){

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

                    std::vector<GLuint> tempIndexes;

                    // RightFace
                    if(blockX + 1 < VoxelWorlds::CHUNK_SIZE) {
                        if(ChunkStorage::GetBlock(*storage, blockX+1, blockY, blockZ) == BlockTypes::air){
                            tempIndexes.insert(tempIndexes.end(), {3,1,7, 5,7,1});
                        }
                    } else if (chunkRight) {
                        if(ChunkStorage::GetBlock(*chunkRight, 0, blockY, blockZ) == BlockTypes::air) {
                            tempIndexes.insert(tempIndexes.end(), {3,1,7, 5,7,1});
                        }
                    }

                    // LeftFace
                    if(blockX - 1 >= 0) {
                        if(ChunkStorage::GetBlock(*storage, blockX-1, blockY, blockZ) == BlockTypes::air){
                            tempIndexes.insert(tempIndexes.end(), {6,4,2, 0,2,4});
                        }
                    } else if (chunkLeft) {
                        if(ChunkStorage::GetBlock(*chunkLeft, VoxelWorlds::CHUNK_SIZE-1, blockY, blockZ) == BlockTypes::air) {
                            tempIndexes.insert(tempIndexes.end(), {6,4,2, 0,2,4});
                        }
                    }

                    // TopFace
                    if(blockY + 1 < VoxelWorlds::CHUNK_SIZE) {
                        if(ChunkStorage::GetBlock(*storage, blockX, blockY+1, blockZ) == BlockTypes::air){
                            tempIndexes.insert(tempIndexes.end(), {6,2,7, 3,7,2});
                        }
                    } else if (chunkTop) {
                        if(ChunkStorage::GetBlock(*chunkTop, blockX, 0, blockZ) == BlockTypes::air) {
                            tempIndexes.insert(tempIndexes.end(), {6,2,7, 3,7,2});
                        }
                    }

                    // BotFace
                    if(blockY - 1 >= 0) {
                        if(ChunkStorage::GetBlock(*storage, blockX, blockY-1, blockZ) == BlockTypes::air){
                            tempIndexes.insert(tempIndexes.end(), {0,4,5, 1,0,5});
                        }
                    } else if (chunkBot) {
                        if(ChunkStorage::GetBlock(*chunkBot, blockX, VoxelWorlds::CHUNK_SIZE-1, blockZ) == BlockTypes::air) {
                            tempIndexes.insert(tempIndexes.end(), {0,4,5, 1,0,5});
                        }
                    }

                    // FrontFace
                    if(blockZ + 1 < VoxelWorlds::CHUNK_SIZE) {
                        if(ChunkStorage::GetBlock(*storage, blockX, blockY, blockZ+1) == BlockTypes::air){
                            tempIndexes.insert(tempIndexes.end(), {0,1,2, 3,2,1}); 
                        }
                    } else if (chunkFront) {
                        if(ChunkStorage::GetBlock(*chunkFront, blockX, blockY, 0) == BlockTypes::air) {
                            tempIndexes.insert(tempIndexes.end(), {0,1,2, 3,2,1}); 
                        }
                    }

                    // BackFace
                    if(blockZ - 1 >= 0) {
                        if(ChunkStorage::GetBlock(*storage, blockX, blockY, blockZ-1) == BlockTypes::air){
                            tempIndexes.insert(tempIndexes.end(), {6,7,4, 7,5,4});
                        }
                    } else if (chunkBack) {
                        if(ChunkStorage::GetBlock(*chunkBack, blockX, blockY, VoxelWorlds::CHUNK_SIZE-1) == BlockTypes::air) {
                            tempIndexes.insert(tempIndexes.end(), {6,7,4, 7,5,4});
                        }
                    }

                    if(tempIndexes.size() == 0) {
                        continue;
                    }
    
                    for(int i = 0; i < tempIndexes.size(); i++) {
                        tempIndexes[i] = tempIndexes[i] + chunkModel.mModel.vertexPositions.size();
                    }
                    
                    auto& blockObject = blockRegistry.GetBlock(block);
                    const glm::vec3 chunkOffset = glm::vec3(blockX,blockY,blockZ);
                    for (auto chunkVertex : blockObject.model.vertexPositions) {
                        chunkModel.mModel.vertexPositions.push_back(chunkVertex + chunkOffset);
                    }
                    
                    chunkModel.mTexturePositions.insert(chunkModel.mTexturePositions.end(), 
                                                    blockObject.model.vertexPositions.begin(),
                                                    blockObject.model.vertexPositions.end());
                    
                    const GLuint64& textureIDs = blockObject.textures->textureHandle;
                    chunkModel.mTextures.insert(chunkModel.mTextures.end(), 
                                                    blockObject.model.vertexPositions.size(),
                                                    textures[textureIDs]);
    
                    chunkModel.mModel.indexBufferData.insert(chunkModel.mModel.indexBufferData.end(), 
                                std::make_move_iterator(tempIndexes.begin()),
                                std::make_move_iterator(tempIndexes.end()));
    
                    BoundingBoxComponent bBox;
                    bBox.mWorldMin = glm::vec3(-0.5f+blockX+VoxelWorlds::CHUNK_SIZE*chunkPos.x,
                                                -0.5f+blockY+VoxelWorlds::CHUNK_SIZE*chunkPos.y,
                                                -0.5f+blockZ+VoxelWorlds::CHUNK_SIZE*chunkPos.z);
    
                    bBox.mWorldMax = glm::vec3( 0.5f+blockX+VoxelWorlds::CHUNK_SIZE*chunkPos.x, 
                                                0.5f+blockY+VoxelWorlds::CHUNK_SIZE*chunkPos.y, 
                                                0.5f+blockZ+VoxelWorlds::CHUNK_SIZE*chunkPos.z);
    
                    bBoxCollection.boundingBoxes.push_back(std::move(bBox));
                }
            }
        }

        *model = std::move(chunkModel);
        model->mGenerated = false;
        
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
std::unordered_map<GLuint64, GLuint> ChunkMeshingSystem::TextureMap() {
    BlockTextureCreator& blockTexture = BlockTextureCreator::GetInstance();
    std::unordered_map<GLuint64, GLuint> textures;

    for (const auto& [_, textureHandle] : blockTexture.GetTextures()) { 
        if (textures.count(textureHandle) == 0) {
            textures[textureHandle];
        }
    }
    GLuint index = 0;
    for(auto& [handle, _] : textures) {
        textures[handle] = index++;
    }
    return textures;
}

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