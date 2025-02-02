#include "world.hpp"

void World::CreateChunk(int chunkX, int chunkY, int chunkZ) {
    auto chunkCoords = std::make_tuple(chunkX,chunkY,chunkZ);
    chunks.emplace(chunkCoords, std::make_shared<Chunk>());
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
                    tempIndexes[i] = tempIndexes[i] + chunkModel.vertexPositions.size();
                }
                
                if(tempIndexes.size() == 0) {
                    continue;
                }

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

    static float cameraOldX = 0;
    static float cameraOldY = 0;
    static float cameraOldZ = 0;

    static int loopX = 0;
    static int loopZ = 0;
    static int max = 1;
    static bool side = true;
    static int chunkY = -mRenderDistance;

    float cameraX = std::floor(mCameraPosition.x/VoxelWorlds::CHUNK_SIZE);
    float cameraY = std::floor(mCameraPosition.y/VoxelWorlds::CHUNK_SIZE);
    float cameraZ = std::floor(mCameraPosition.z/VoxelWorlds::CHUNK_SIZE);

    if(cameraX != cameraOldX || cameraY != cameraOldY || cameraZ != cameraOldZ) {
        loopX = 0;
        loopZ = 0;
        max = 1;
        side = true;
        chunkY = -mRenderDistance;
    }

    cameraOldX = cameraX;
    cameraOldY = cameraY;
    cameraOldZ = cameraZ;

    int coordinatesX = cameraX + loopX;
    int coordinatesY = cameraY + chunkY;
    int coordinatesZ = cameraZ + loopZ;

    if(!GetChunk(coordinatesX,coordinatesY,coordinatesZ)) {
        CreateChunk(coordinatesX,coordinatesY,coordinatesZ);
        auto chunk = GetChunk(coordinatesX,coordinatesY,coordinatesZ);
        utility::MeshTranslate(chunk->mTransform, glm::vec3(static_cast<float>(coordinatesX)*VoxelWorlds::CHUNK_SIZE, 
                                                            static_cast<float>(coordinatesY)*VoxelWorlds::CHUNK_SIZE,
                                                            static_cast<float>(coordinatesZ)*VoxelWorlds::CHUNK_SIZE));
                    
    }

    if(chunkY <= mRenderDistance) {
        chunkY++;
    } else {
        chunkY = -mRenderDistance;

        int chunkCoordinateX = static_cast<int>(std::floor(static_cast<float>(coordinatesX)/VoxelWorlds::PERLIN_SCALE));
        int chunkCoordinateZ = static_cast<int>(std::floor(static_cast<float>(coordinatesZ)/VoxelWorlds::PERLIN_SCALE));

        int xOffset = (coordinatesX % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;
        int zOffset = (coordinatesZ % VoxelWorlds::PERLIN_SCALE + VoxelWorlds::PERLIN_SCALE) % VoxelWorlds::PERLIN_SCALE;

        int xOffset1 = (coordinatesX % (VoxelWorlds::PERLIN_SCALE+3) + VoxelWorlds::PERLIN_SCALE+3) % (VoxelWorlds::PERLIN_SCALE+3);
        int zOffset1 = (coordinatesZ % (VoxelWorlds::PERLIN_SCALE+3) + VoxelWorlds::PERLIN_SCALE+3) % (VoxelWorlds::PERLIN_SCALE+3);
        
        std::shared_ptr<Chunk> chunk;
        for(float x = 0; x < VoxelWorlds::CHUNK_SIZE; x++) {
            for(float z = 0; z < VoxelWorlds::CHUNK_SIZE; z++) {
                if(chunk && chunk->wasGenerated) {
                    continue;
                }

                float height = utility::LayeredPerlinNoise(
                    chunkCoordinateX,
                    chunkCoordinateZ,
                    (x+(xOffset*VoxelWorlds::CHUNK_SIZE))/(VoxelWorlds::CHUNK_SIZE*VoxelWorlds::PERLIN_SCALE),
                    (z+(zOffset*VoxelWorlds::CHUNK_SIZE))/(VoxelWorlds::CHUNK_SIZE*VoxelWorlds::PERLIN_SCALE),
                    mSeed,
                    VoxelWorlds::OCTAVES,
                    VoxelWorlds::PERSISTANCE,
                    VoxelWorlds::LACUNARITY
                );
                height = 1 - height;
                height = std::round(std::pow(height,4) * 1000);

                int numChunks = static_cast<int>(height / VoxelWorlds::CHUNK_SIZE);
                int remainder = static_cast<int>(height) % static_cast<int>(VoxelWorlds::CHUNK_SIZE); // Extra blocks for the top chunk

                for (int i = 0; i <= numChunks; i++) {
                    coordinatesY = i;
                    chunk = GetChunk(coordinatesX,coordinatesY,coordinatesZ);

                    int blocksToPlace = (i == numChunks) ? remainder : VoxelWorlds::CHUNK_SIZE;

                    if(!chunk || chunk->wasGenerated) {
                        continue;
                    }
                    if(x == VoxelWorlds::CHUNK_SIZE-1 && z == VoxelWorlds::CHUNK_SIZE-1) {
                        chunk->wasGenerated = true;
                    }

                    for(float y = 0; y < blocksToPlace; y++) {
                        int globalY = (i * VoxelWorlds::CHUNK_SIZE) + y;
                        if(globalY == height-1) {
                            chunkManger.InsertToChunk(*chunk, BlockTypes::grass_block, x, y, z);
                        } else {
                            chunkManger.InsertToChunk(*chunk, BlockTypes::dirt_block, x, y, z);
                        }
                    }
                }
            }
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

        if(max <= mRenderDistance+VoxelWorlds::CHUNK_GENERATION_OFFSET) {
            if(loopZ == -max) {
                max++;
            } 
        } else if(loopX >= max){
            loopX = 0;
            loopZ = 0;
            max = 1;
            side = true;
            chunkY = -mRenderDistance;
        }

    }
}
// TO-DO make this better
void World::GenerateMesh() {
    ChunkManager chunkManger;
    static uint8_t delay = 122; // In frames

    if(delay > 0) {
        delay--;
        return;
    }
    
    static float cameraOldX = 0;
    static float cameraOldY = 0;
    static float cameraOldZ = 0;

    static int loopX = 0;
    static int loopZ = 0;
    static int max = 1;
    static bool side = true;
    static int chunkY = -mRenderDistance;

    float cameraX = std::floor(mCameraPosition.x/VoxelWorlds::CHUNK_SIZE);
    float cameraY = std::floor(mCameraPosition.y/VoxelWorlds::CHUNK_SIZE);
    float cameraZ = std::floor(mCameraPosition.z/VoxelWorlds::CHUNK_SIZE);

    if(cameraX != cameraOldX || cameraY != cameraOldY || cameraZ != cameraOldZ) {
        loopX = 0;
        loopZ = 0;
        max = 1;
        side = true;
        chunkY = -mRenderDistance;
    }

    cameraOldX = cameraX;
    cameraOldY = cameraY;
    cameraOldZ = cameraZ;

    int coordinatesX = cameraX + loopX;
    int coordinatesY = cameraY + chunkY;
    int coordinatesZ = cameraZ + loopZ;
    
    auto chunk = GetChunk(coordinatesX,coordinatesY,coordinatesZ);

    if(chunk) {
        if(chunk->mVertexArrayObject == 0) {
            CreateChunkModel(coordinatesX,coordinatesY,coordinatesZ);
            chunkManger.CreateVAO(*chunk);
        }        
    }

    if(chunkY < mRenderDistance) {
        chunkY++;
    } else {
        chunkY = -mRenderDistance;
        
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

        if(max <= mRenderDistance) {
            if(loopZ == -max) {
                max++;
            } 
        } else if(loopX >= max){
            loopX = 0;
            loopZ = 0;
            max = 1;
            side = true;
            chunkY = -mRenderDistance;
        }
    }
}

void World::DrawChunks() {
    ChunkRendererSystem& gChunkRendererSystem = ChunkRendererSystem::GetInstance();

    float cameraX = std::floor(mCameraPosition.x/VoxelWorlds::CHUNK_SIZE);
    float cameraY = std::floor(mCameraPosition.y/VoxelWorlds::CHUNK_SIZE);
    float cameraZ = std::floor(mCameraPosition.z/VoxelWorlds::CHUNK_SIZE);

    for (int xChunkPos = cameraX - mRenderDistance; xChunkPos < cameraX + mRenderDistance; xChunkPos++) {
        for (int zChunkPos = cameraZ - mRenderDistance; zChunkPos < cameraZ + mRenderDistance; zChunkPos++) {
            for (int yChunkPos = cameraY - mRenderDistance; yChunkPos < cameraY + mRenderDistance; yChunkPos++) {
                auto chunk = GetChunk(xChunkPos, yChunkPos, zChunkPos);
                gChunkRendererSystem.DrawChunk(chunk);
            }
        }
    }
}
