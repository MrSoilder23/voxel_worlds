#include "./systems/world_generation_system.hpp"

void WorldGenerationSystem::SetEntityManager(EntityManager& entityManager) {
    mEntityManager = &entityManager;
}
void WorldGenerationSystem::SetSeed(unsigned int seed) {
    mSeed = seed;
}
#include <iostream>
void WorldGenerationSystem::GenerateChunk(int x, int y, int z) {
    char chunkName[32];
    snprintf(chunkName, sizeof(chunkName), "%d:%d:%d", x, y, z);

    if(!mEntityManager->CreateEntity(chunkName)) {
        return;
    }
    mEntityManager->AddComponent<PositionComponent>(chunkName);
    mEntityManager->AddComponent<ChunkModelComponent>(chunkName);
    mEntityManager->AddComponent<ChunkStorageComponent>(chunkName);
    mEntityManager->AddComponent<BoundingBoxComponent>(chunkName);
    
    BoundingBoxComponent bBoxComponent;
    bBoxComponent.mMin = glm::vec3(-0.5f,-0.5f,-0.5);
    bBoxComponent.mMax = glm::vec3(VoxelWorlds::CHUNK_SIZE-0.5f,VoxelWorlds::CHUNK_SIZE-0.5f,VoxelWorlds::CHUNK_SIZE-0.5f);
    
    PositionComponent posComponent;
    utility::MovePosition(posComponent, glm::vec3(x*VoxelWorlds::CHUNK_SIZE,
        y*VoxelWorlds::CHUNK_SIZE, z*VoxelWorlds::CHUNK_SIZE));
        
        
    auto boundingBox = mEntityManager->GetComponent<BoundingBoxComponent>(chunkName);
    auto position = mEntityManager->GetComponent<PositionComponent>(chunkName);

    *boundingBox = std::move(bBoxComponent);
    *position = std::move(posComponent);

    GeneratePerlin(x,y,z, chunkName);
}
void WorldGenerationSystem::GenerateModel(int x, int y, int z) {
    char chunkName[32];
    snprintf(chunkName, sizeof(chunkName), "%d:%d:%d", x, y, z);

    auto chunkData = mEntityManager->GetComponent<ChunkStorageComponent>(chunkName);
    if(!chunkData->mWasGenerated) {
        return;
    }
    
    auto chunkModel = mEntityManager->GetComponent<ChunkModelComponent>(chunkName);

    // Texture mapping
    BlockRegistry& blockRegistry = BlockRegistry::GetInstance();
    BlockTextureCreator& blockTexture = BlockTextureCreator::GetInstance();
    std::unordered_map<GLuint64, GLuint> textures;

    Model model;
    std::vector<GLuint> textureIdVector;
    std::vector<glm::vec3> texturePositions;
    
    for (const auto& [_, textureHandle] : blockTexture.GetTextures()) { 
        if (textures.count(textureHandle) == 0) {
            textures[textureHandle];
        }
    }
    GLuint index = 0;
    for(auto& [handle, _] : textures) {
        textures[handle] = index++;
    }
    
    for(int blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
        for(int blockY = 0; blockY < VoxelWorlds::CHUNK_SIZE; blockY++) {
            for(int blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {
                BlockTypes block = ChunkStorage::GetBlock(*chunkData, blockX, blockY, blockZ);
                
                if(block == BlockTypes::air) {
                    continue;
                }
                
                std::vector<GLuint> tempIndexes;
                
                if(CheckBlock(*chunkData, x,y,z, blockX+1, blockY, blockZ)) {
                    tempIndexes.insert(tempIndexes.end(), {3,1,7, 5,7,1}); // RightFace
                }
                if(CheckBlock(*chunkData, x,y,z, blockX-1, blockY, blockZ)) {
                    tempIndexes.insert(tempIndexes.end(), {6,4,2, 0,2,4}); // LeftFace
                }
                if(CheckBlock(*chunkData, x,y,z, blockX, blockY+1, blockZ)) {
                    tempIndexes.insert(tempIndexes.end(), {6,2,7, 3,7,2}); // TopFace
                }
                if(CheckBlock(*chunkData, x,y,z, blockX, blockY-1, blockZ)) {
                    tempIndexes.insert(tempIndexes.end(), {0,4,5, 1,0,5}); // BotFace
                }
                if(CheckBlock(*chunkData, x,y,z, blockX, blockY, blockZ+1)) {
                    tempIndexes.insert(tempIndexes.end(), {0,1,2, 3,2,1}); // FrontFace
                }
                if(CheckBlock(*chunkData, x,y,z, blockX, blockY, blockZ-1)) {
                    tempIndexes.insert(tempIndexes.end(), {6,7,4, 7,5,4}); // BackFace
                }
                                
                if(tempIndexes.size() == 0) {
                    continue;
                }

                for(int i = 0; i < tempIndexes.size(); i++) {
                    tempIndexes[i] = tempIndexes[i] + model.vertexPositions.size();
                }
                
                auto& blockObject = blockRegistry.GetBlock(block);
                glm::vec3 chunkOffset = glm::vec3(blockX,blockY,blockZ);
                for (auto chunkVertex : blockObject.model.vertexPositions) {
                    model.vertexPositions.push_back(chunkVertex + chunkOffset);
                }
                
                texturePositions.insert(texturePositions.end(), 
                                                blockObject.model.vertexPositions.begin(),
                                                blockObject.model.vertexPositions.end());
                
                GLuint64 textureIDs = blockObject.textures->textureHandle;
                textureIdVector.insert(textureIdVector.end(), 
                                                blockObject.model.vertexPositions.size(),
                                                textures[textureIDs]);

                model.indexBufferData.insert(model.indexBufferData.end(), 
                            std::make_move_iterator(tempIndexes.begin()),
                            std::make_move_iterator(tempIndexes.end()));
            }
        }
    }
    
    if(chunkModel->mChangeSize != model.indexBufferData.size()) {
        chunkModel->mTexturePositions.clear();
        chunkModel->mTextures.clear();
        chunkModel->mModel = Model();

        chunkModel->mTexturePositions = std::move(texturePositions);
        chunkModel->mTextures = std::move(textureIdVector);
        chunkModel->mModel = std::move(model);

        chunkModel->mChangeSize = chunkModel->mModel.indexBufferData.size();
        chunkModel->mGenerated = false;
    }
}

// Private functions
void WorldGenerationSystem::GeneratePerlin(int x, int y, int z, std::string chunkName) {
    auto chunkData = mEntityManager->GetComponent<ChunkStorageComponent>(chunkName);
    if(!chunkData || chunkData->mWasGenerated) {
        return;
    }

    int chunkCoordinateX = static_cast<int>(std::floor(static_cast<float>(x)/VoxelWorlds::PERLIN_SCALE));
    int chunkCoordinateZ = static_cast<int>(std::floor(static_cast<float>(z)/VoxelWorlds::PERLIN_SCALE));

    int xOffset = (x % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;
    int zOffset = (z % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;
    
    ChunkStorageComponent chunkStorage;
    chunkStorage = *chunkData;

    for(float blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
        for(float blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {

            float height = utility::LayeredPerlinNoise(
                chunkCoordinateX,
                chunkCoordinateZ,
                (blockX+(xOffset*VoxelWorlds::CHUNK_SIZE))/(VoxelWorlds::CHUNK_SIZE*VoxelWorlds::PERLIN_SCALE),
                (blockZ+(zOffset*VoxelWorlds::CHUNK_SIZE))/(VoxelWorlds::CHUNK_SIZE*VoxelWorlds::PERLIN_SCALE),
                mSeed,
                VoxelWorlds::OCTAVES,
                VoxelWorlds::PERSISTANCE,
                VoxelWorlds::LACUNARITY
            );
            height = 1 - height;
            height = std::round(std::pow(height,4) * 600);

            int numChunks = static_cast<int>(height / VoxelWorlds::CHUNK_SIZE);
            int remainder = static_cast<int>(height) % static_cast<int>(VoxelWorlds::CHUNK_SIZE); // Extra blocks for the top chunk

            int blocksToPlace = (y < numChunks) ? VoxelWorlds::CHUNK_SIZE : 0;
            if(y == numChunks) {
                blocksToPlace = remainder;
            }

            for(float blockY = 0; blockY < blocksToPlace; blockY++) {
                int globalY = (y * VoxelWorlds::CHUNK_SIZE) + blockY;
                if(globalY == height-1) {
                    ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::grass_block, blockX, blockY, blockZ);
                } else if(globalY <= height-2 && globalY >= height-5){
                    ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::dirt_block, blockX, blockY, blockZ);
                } else {
                    ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::stone_block, blockX, blockY, blockZ);
                }
            }
        }
    }
    chunkStorage.mWasGenerated = true;
    *chunkData = std::move(chunkStorage);
}

bool WorldGenerationSystem::CheckBlock(ChunkStorageComponent& currentChunkData, int chunkX, int chunkY, int chunkZ, int x, int y, int z) {
    int chunkSum = chunkX + chunkY + chunkZ;
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
    
    int chunkSum1 = chunkX + chunkY + chunkZ;
    char chunkName[32];
    if(chunkSum1 != chunkSum) {
        snprintf(chunkName, sizeof(chunkName), "%d:%d:%d", chunkX, chunkY, chunkZ);
        auto chunkData = mEntityManager->GetComponent<ChunkStorageComponent>(chunkName);

        if(!chunkData) {
            return false;
        }
        
        if(ChunkStorage::GetBlock(*chunkData, x,y,z) == BlockTypes::air) {
            return true; 
        }
        return false;
    }

    if(ChunkStorage::GetBlock(currentChunkData, x,y,z) == BlockTypes::air) {
        return true; 
    }

    return false;
}