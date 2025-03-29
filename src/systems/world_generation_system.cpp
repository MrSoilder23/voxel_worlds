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
    FastChunkName(chunkName, x, y, z);

    if(!mEntityManager->CreateEntity(chunkName)) {
        return;
    }
    
    BoundingBoxComponent bBoxComponent;
    bBoxComponent.group = Group::render;
    bBoxComponent.mLocalMin = glm::vec3(-0.5f,-0.5f,-0.5f);
    bBoxComponent.mLocalMax = glm::vec3(VoxelWorlds::CHUNK_SIZE-0.5f,
                                   VoxelWorlds::CHUNK_SIZE-0.5f,
                                   VoxelWorlds::CHUNK_SIZE-0.5f);
    
    PositionComponent posComponent;
    utility::MovePosition(posComponent, glm::vec3(x*VoxelWorlds::CHUNK_SIZE,
        y*VoxelWorlds::CHUNK_SIZE, z*VoxelWorlds::CHUNK_SIZE));

    mEntityManager->AddComponent<PositionComponent>(chunkName, posComponent);
    mEntityManager->AddComponent<ChunkStorageComponent>(chunkName);
    mEntityManager->AddComponent<ChunkModelComponent>(chunkName);
    mEntityManager->AddComponent<BoundingBoxComponent>(chunkName, bBoxComponent);
    mEntityManager->AddComponent<BoundingBoxCollectionComponent>(chunkName);

    GenerateNoise(x,y,z, chunkName);
}
void WorldGenerationSystem::GenerateModel(int x, int y, int z) {
    char chunkName[32];
    FastChunkName(chunkName, x, y, z);

    auto chunkData = mEntityManager->GetComponent<ChunkStorageComponent>(chunkName);
    if(!chunkData->mWasGenerated) {
        return;
    }

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

    BoundingBoxCollectionComponent bBoxCollection;
    
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

                BoundingBoxComponent bBox;
                bBox.mWorldMin = glm::vec3(-0.5f+blockX+VoxelWorlds::CHUNK_SIZE*x,
                                           -0.5f+blockY+VoxelWorlds::CHUNK_SIZE*y,
                                           -0.5f+blockZ+VoxelWorlds::CHUNK_SIZE*z);

                bBox.mWorldMax = glm::vec3( 0.5f+blockX+VoxelWorlds::CHUNK_SIZE*x, 
                                            0.5f+blockY+VoxelWorlds::CHUNK_SIZE*y, 
                                            0.5f+blockZ+VoxelWorlds::CHUNK_SIZE*z);

                bBoxCollection.boundingBoxes.push_back(std::move(bBox));
            }
        }
    }
    
    auto chunkModel = mEntityManager->GetComponent<ChunkModelComponent>(chunkName);
    if(chunkModel->mModel.indexBufferData.size() != model.indexBufferData.size() || chunkModel->mModel.vertexPositions.size() != model.indexBufferData.size()) {
        chunkModel->mTexturePositions.clear();
        chunkModel->mTextures.clear();
        chunkModel->mModel = Model();
        
        chunkModel->mTexturePositions = std::move(texturePositions);
        chunkModel->mTextures = std::move(textureIdVector);
        chunkModel->mModel = std::move(model);
        
        chunkModel->mGenerated = false;
        
        auto chunkBoxes = mEntityManager->GetComponent<BoundingBoxCollectionComponent>(chunkName);
        *chunkBoxes = std::move(bBoxCollection);
    }
}

// Private functions
void WorldGenerationSystem::GenerateNoise(int x, int y, int z, std::string chunkName) {
    auto chunkData = mEntityManager->GetComponent<ChunkStorageComponent>(chunkName);
    static float chunkCoords = VoxelWorlds::CHUNK_SIZE-1.0f;

    ChunkStorageComponent chunkStorage;
    chunkStorage = *chunkData;

    for(float blockX = 0; blockX < VoxelWorlds::CHUNK_SIZE; blockX++) {
        for(float blockZ = 0; blockZ < VoxelWorlds::CHUNK_SIZE; blockZ++) {
            //
            float noise = open_simplex_noise::LayeredNoise2D(
                (blockX + (x * VoxelWorlds::CHUNK_SIZE)) * (0.005f / (2 * VoxelWorlds::SCALE)),
                (blockZ + (z * VoxelWorlds::CHUNK_SIZE)) * (0.005f / (2 * VoxelWorlds::SCALE)),
                mSeed,
                VoxelWorlds::OCTAVES,
                VoxelWorlds::PERSISTANCE + 0.3f,
                VoxelWorlds::LACUNARITY + 2
            );

            float height = GenerateHeight(blockX + (x * VoxelWorlds::CHUNK_SIZE),
                                          blockZ + (z * VoxelWorlds::CHUNK_SIZE));

            height = std::round(height);

            int numChunks = static_cast<int>(height / VoxelWorlds::CHUNK_SIZE);
            int remainder = static_cast<int>(height) % static_cast<int>(VoxelWorlds::CHUNK_SIZE); // Extra blocks for the top chunk

            int blocksToPlace = (y < numChunks) ? VoxelWorlds::CHUNK_SIZE : 0;
            if(y == numChunks) {
                blocksToPlace = remainder;
            }

            for(float blockY = 0; blockY < blocksToPlace; blockY++) {

                int globalY = (y * VoxelWorlds::CHUNK_SIZE) + blockY;

                if(globalY == height-1) {
                    if(globalY < 100.0f) {
                        if(noise >= 0.0f && noise <= 0.6f) {
                            ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::dirt_block, blockX, blockY, blockZ);
                        } else {
                            ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::sand_block, blockX, blockY, blockZ);
                        }
         
                    } else if(globalY < 110.0f) {
                        ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::sand_block, blockX, blockY, blockZ);
                    } else {
                        ChunkStorage::InsertToChunk(chunkStorage, BlockTypes::grass_block, blockX, blockY, blockZ);
                    }

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

float WorldGenerationSystem::GenerateHeight(int x, int z) {
    float continentalness = open_simplex_noise::LayeredNoise2D(
        x * (0.005f / (4 * VoxelWorlds::SCALE)),
        z * (0.005f / (4 * VoxelWorlds::SCALE)),
        mSeed,
        4,
        0.5f,
        2.5f
    );
    float erosion = open_simplex_noise::LayeredNoise2D(
        x * (0.004f / (4 * VoxelWorlds::SCALE)),
        z * (0.004f / (4 * VoxelWorlds::SCALE)),
        mSeed,
        4,
        0.4f,
        2.4f
    );
    float peaksAndValleys = open_simplex_noise::LayeredNoise2D(
        x * (0.025f / (10 * VoxelWorlds::SCALE)),
        z * (0.025f / (10 * VoxelWorlds::SCALE)),
        mSeed,
        6,
        0.4f,
        1.8f
    );

    float continentalAdj = VoxelWorlds::CONTINENTAL_SPLINE.evaluate(continentalness);
    float erosionAdj = VoxelWorlds::EROSION_SPLINE.evaluate(erosion);
    float peaksValleysAdj = VoxelWorlds::PEAKS_VALLEYS_SPLINE.evaluate(peaksAndValleys);

    return (continentalAdj * 0.5f) + (erosionAdj * 0.3f) + (peaksValleysAdj * 0.2f);
}

inline bool WorldGenerationSystem::CheckBlock(ChunkStorageComponent& currentChunkData, int chunkX, int chunkY, int chunkZ, int x, int y, int z) {
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
        FastChunkName(chunkName, chunkX, chunkY, chunkZ);

        if(auto chunkData = mEntityManager->GetComponent<ChunkStorageComponent>(chunkName)) {
            return ChunkStorage::GetBlock(*chunkData, x,y,z) == BlockTypes::air;
        }
        
        return false;
    }

    return ChunkStorage::GetBlock(currentChunkData, x,y,z) == BlockTypes::air;
}

inline char* WorldGenerationSystem::FastIntToString(char* ptr, int value) {
    if (value == 0) {
        *ptr++ = '0';
        return ptr;
    }

    const bool negative = value < 0;
    if (negative) {
        *ptr++ = '-';
        value = -value;
    }

    char buffer[16];
    int i = 0;
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i > 0) {
        *ptr++ = buffer[--i];
    }

    return ptr;
}


void WorldGenerationSystem::FastChunkName(char* ptr, int chunkX, int chunkY, int chunkZ) {
    ptr = FastIntToString(ptr, chunkX);
    *ptr++ = ':';
    ptr = FastIntToString(ptr, chunkY);
    *ptr++ = ':';
    ptr = FastIntToString(ptr, chunkZ);
    *ptr++ = '\0';
}