#include "world.hpp"

void World::CreateChunk(int chunkX, int chunkY, int chunkZ) {
    auto chunkCoords = std::make_tuple(chunkX,chunkY,chunkZ);
    chunks[chunkCoords] = std::make_shared<Chunk>();
}
std::shared_ptr<Chunk> World::GetChunk(int chunkX, int chunkY, int chunkZ) const {
    auto chunkCoords = std::make_tuple(chunkX,chunkY,chunkZ);
    auto findChunk = chunks.find(chunkCoords);
    if(findChunk == chunks.end()) {
        return nullptr;
    }

    return findChunk->second;
}

BlockTypes World::GetBlock(int chunkX, int chunkY, int chunkZ, int x, int y, int z) {
    if(x < 0) {
        x = 31;
        chunkX--;
    } else if(x > 31) {
        x = 0;
        chunkX++;
    } else if(y < 0) {
        y = 31;
        chunkY--;
    } else if(y > 31) {
        y = 0;
        chunkY++;
    } else if(z < 0) {
        z = 31;
        chunkZ--;
    } else if(z > 31) {
        z = 0;
        chunkZ++;
    }
    ChunkKey key = std::make_tuple(chunkX,chunkY,chunkZ);

    if(chunks.find(key) == chunks.end()) {
        return BlockTypes::air; 
    }

    return chunks[key]->blocks[x][y][z];
}

void World::CreateChunkModel(int chunkX, int chunkY, int chunkZ) {
    EntityManager& entityManager = EntityManager::GetInstance();
    BlockTextureCreator& blockTexture = BlockTextureCreator::GetInstance();

    Model chunkModel;
    std::vector<GLuint> indexes;
    ChunkKey key = std::make_tuple(chunkX,chunkY,chunkZ);


    auto& textureList = blockTexture.GetTextures();
    
    for (const auto& [_, textureHandle] : textureList) { 
        if (chunks[key]->mTextures.count(textureHandle) == 0) {
            chunks[key]->mTextures[textureHandle];
        }
    }
    GLuint index = 0;
    for(auto& [handle, _] : chunks[key]->mTextures) {
        chunks[key]->mTextures[handle] = index++;
    }

    for(int x = 0; x < VoxelWorlds::CHUNK_SIZE; x++) {
        for(int y = 0; y < VoxelWorlds::CHUNK_SIZE; y++) {
            for(int z = 0; z < VoxelWorlds::CHUNK_SIZE; z++) {
                auto chunk = chunks[key];
                BlockTypes block = chunk->blocks[x][y][z];

                if(block == BlockTypes::air) {
                    continue;
                }
                
                std::vector<GLuint> tempIndexes;
                
                auto modelComponent = entityManager.GetComponent<ModelComponent>(block);
                glm::vec3 chunkOffset = glm::vec3(x,y,z);
                for (auto chunkVertex : modelComponent->mModel.vertexPositions) {
                    chunkModel.vertexPositions.push_back(chunkVertex + chunkOffset);
                }
                
                chunk->mTexturePositions.insert(chunk->mTexturePositions.end(), 
                                                modelComponent->mModel.vertexPositions.begin(),
                                                modelComponent->mModel.vertexPositions.end());

                auto textureIDs = modelComponent->mTextures->textureHandle;
                chunk->mTextureID.insert(chunk->mTextureID.end(), 
                                                modelComponent->mModel.vertexPositions.size(),
                                                chunk->mTextures[textureIDs]);

                if(GetBlock(chunkX,chunkY,chunkZ ,x+1, y, z) == BlockTypes::air) {
                    tempIndexes.insert(tempIndexes.end(), {3,1,7, 5,7,1}); // RightFace
                }
                if(GetBlock(chunkX,chunkY,chunkZ ,x-1, y, z) == BlockTypes::air) {
                    tempIndexes.insert(tempIndexes.end(), {6,4,2, 0,2,4}); // LeftFace
                }
                if(GetBlock(chunkX,chunkY,chunkZ ,x, y+1, z) == BlockTypes::air) {
                    tempIndexes.insert(tempIndexes.end(), {6,2,7, 3,7,2}); // TopFace
                }
                if(GetBlock(chunkX,chunkY,chunkZ ,x, y-1, z) == BlockTypes::air) {
                    tempIndexes.insert(tempIndexes.end(), {0,4,5, 1,0,5}); // BotFace
                }
                if(GetBlock(chunkX,chunkY,chunkZ ,x, y, z+1) == BlockTypes::air) {
                    tempIndexes.insert(tempIndexes.end(), {0,1,2, 3,2,1}); // FrontFace
                }
                if(GetBlock(chunkX,chunkY,chunkZ ,x, y, z-1) == BlockTypes::air) {
                    tempIndexes.insert(tempIndexes.end(), {6,7,4, 7,5,4}); // BackFace
                }

                for(int i = 0; i < tempIndexes.size(); i++) {
                    tempIndexes[i] = tempIndexes[i] + chunkModel.vertexPositions.size()-8;
                }
                indexes.insert(indexes.end(), 
                            std::make_move_iterator(tempIndexes.begin()),
                            std::make_move_iterator(tempIndexes.end()));
            }
        }
    }
    chunkModel.indexBufferData = std::move(indexes);
    chunks[key]->mModel = std::move(chunkModel);
}

void World::SetCameraPosition(glm::vec3 cameraPosition) {
    mCameraPosition = cameraPosition;
}
void World::SetRenderDistance(float renderDistance) {
    mRenderDistance = renderDistance;
}
void World::SetSeed(unsigned int seed) {
    mSeed = seed;
}

void World::GenerateWorld() {
    ChunkManager chunkManger;

    float cameraX = std::floor(mCameraPosition.x/VoxelWorlds::CHUNK_SIZE);
    float cameraZ = std::floor(mCameraPosition.z/VoxelWorlds::CHUNK_SIZE);

    static int loopX = 0;
    static int loopZ = 0;
    static int max = 1;
    static bool side = true;

    int coordinatesX = cameraX + loopX;
    int coordinatesZ = cameraZ + loopZ;

    if(GetChunk(coordinatesX,0,coordinatesZ) == nullptr) {
        CreateChunk(coordinatesX,0,coordinatesZ);
        auto chunk = GetChunk(coordinatesX,0,coordinatesZ);
        utility::MeshTranslate(chunk->mTransform, glm::vec3(static_cast<float>(coordinatesX)*VoxelWorlds::CHUNK_SIZE, 0,static_cast<float>(coordinatesZ)*VoxelWorlds::CHUNK_SIZE));
        
        for(float x = 0; x < VoxelWorlds::CHUNK_SIZE; x++) {
            for(float z = 0; z < VoxelWorlds::CHUNK_SIZE; z++) {
                float height = std::round(utility::PerlinNoise(coordinatesX,coordinatesZ , x/VoxelWorlds::CHUNK_SIZE, z/VoxelWorlds::CHUNK_SIZE, mSeed) * VoxelWorlds::CHUNK_SIZE);

                for(float y = 0; y < height; y++) {
                    if(y == height-1) {
                        chunkManger.InsertToChunk(*chunk, BlockTypes::grass_block, x, y, z);
                    } else {
                        chunkManger.InsertToChunk(*chunk, BlockTypes::dirt_block, x, y, z);
                    }
                }
            }
        }
        // To-Do pre generate chunks and then initialize chunk in the middle
        CreateChunkModel(coordinatesX,0,coordinatesZ);
        chunkManger.CreateVAO(*chunk);
    }

    // Spiral loop
    if(side) {
        if(loopX < max) {
            loopX++;
        } else if(loopZ < max){
            loopZ++;
        }
        if(loopZ == max) {
            side = false;
        }
    } else {
        if(loopX > -max) {
            loopX--;
        } else if(loopZ > -max){
            loopZ--;
        } 
        if(loopZ == -max) {
            side = true;
        }
    }

    if(max <= mRenderDistance+VoxelWorlds::CHUNK_OFFSET) {
        if(loopZ == -max) {
            max++;
        } 
    } else if(loopX >= max){
        loopX = 0;
        loopZ = 0;
        max = 1;
        side = true;
    }
}

void World::DrawChunks() {
    ChunkRendererSystem& gChunkRendererSystem = ChunkRendererSystem::GetInstance();

    float cameraX = std::floor(mCameraPosition.x/VoxelWorlds::CHUNK_SIZE);
    float cameraZ = std::floor(mCameraPosition.z/VoxelWorlds::CHUNK_SIZE);

    for (int xChunkPos = cameraX - mRenderDistance; xChunkPos < cameraX + mRenderDistance; xChunkPos++) {
        for (int zChunkPos = cameraZ - mRenderDistance; zChunkPos < cameraZ + mRenderDistance; zChunkPos++) {
            auto chunk = GetChunk(xChunkPos, 0, zChunkPos);
            gChunkRendererSystem.DrawChunk(chunk);
        }
    }
}
