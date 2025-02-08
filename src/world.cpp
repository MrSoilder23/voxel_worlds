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
    auto chunk = chunks[key];
    chunk->mTexturePositions.clear();
    chunk->mTextureID.clear();
    chunk->mModel = Model();

    for(int x = 0; x < VoxelWorlds::CHUNK_SIZE; x++) {
        for(int y = 0; y < VoxelWorlds::CHUNK_SIZE; y++) {
            for(int z = 0; z < VoxelWorlds::CHUNK_SIZE; z++) {
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
    chunk->mModel = std::move(chunkModel);
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
void World::GenerateChunks(int loopX, int loopZ) {
    float cameraX = std::floor(mCameraPosition.x/VoxelWorlds::CHUNK_SIZE);
    float cameraY = std::floor(mCameraPosition.y/VoxelWorlds::CHUNK_SIZE);
    float cameraZ = std::floor(mCameraPosition.z/VoxelWorlds::CHUNK_SIZE);

    int coordinatesX = cameraX + loopX;
    int coordinatesY = cameraY;
    int coordinatesZ = cameraZ + loopZ;

    for(int chunkY = -mRenderDistance; chunkY <= mRenderDistance; chunkY++) {
        coordinatesY = cameraY + chunkY;
        if(!GetChunk(coordinatesX,coordinatesY,coordinatesZ)) {
            CreateChunk(coordinatesX,coordinatesY,coordinatesZ);    
            auto chunk = GetChunk(coordinatesX,coordinatesY,coordinatesZ);
            utility::MeshTranslate(chunk->mTransform, glm::vec3(static_cast<float>(coordinatesX)*VoxelWorlds::CHUNK_SIZE, 
                                                                static_cast<float>(coordinatesY)*VoxelWorlds::CHUNK_SIZE,
                                                                static_cast<float>(coordinatesZ)*VoxelWorlds::CHUNK_SIZE));
                        
        }
    }
}

void World::GenerateWorld(int loopX, int loopZ) {
    float cameraX = std::floor(mCameraPosition.x/VoxelWorlds::CHUNK_SIZE);
    float cameraY = std::floor(mCameraPosition.y/VoxelWorlds::CHUNK_SIZE);
    float cameraZ = std::floor(mCameraPosition.z/VoxelWorlds::CHUNK_SIZE);

    int coordinatesX = cameraX + loopX;
    int coordinatesY = cameraY;
    int coordinatesZ = cameraZ + loopZ;

    GenerateWorldChunk(coordinatesX, coordinatesY, coordinatesZ);
}
// TO-DO make this better
void World::GenerateMesh(int loopX, int loopZ) {
    float cameraX = std::floor(mCameraPosition.x/VoxelWorlds::CHUNK_SIZE);
    float cameraY = std::floor(mCameraPosition.y/VoxelWorlds::CHUNK_SIZE);
    float cameraZ = std::floor(mCameraPosition.z/VoxelWorlds::CHUNK_SIZE);

    int coordinatesX = cameraX + loopX;
    int coordinatesZ = cameraZ + loopZ;

    ChunkManager chunkManger;

    for(int chunkY = -mRenderDistance; chunkY <= mRenderDistance; chunkY++) {
        int coordinatesY = cameraY + chunkY;
        auto chunk = GetChunk(coordinatesX,coordinatesY,coordinatesZ);

        if(chunk && chunk->wasGenerated && chunk->mVertexArrayObject == 0) {
            CreateChunkModel(coordinatesX,coordinatesY,coordinatesZ);
        }        
    }
}

void World::GenerateWorldChunk(int coordinatesX,int coordinatesY,int coordinatesZ) {
    ChunkManager chunkManger;

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
                continue;;
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
            height = std::round(std::pow(height,4) * 600);

            int numChunks = static_cast<int>(height / VoxelWorlds::CHUNK_SIZE);
            int remainder = static_cast<int>(height) % static_cast<int>(VoxelWorlds::CHUNK_SIZE); // Extra blocks for the top chunk

            for (int i = 0; i <= mRenderDistance; i++) {
                coordinatesY = i;
                chunk = GetChunk(coordinatesX,coordinatesY,coordinatesZ);
                
                int blocksToPlace = (i < numChunks) ? VoxelWorlds::CHUNK_SIZE : 0;
                if(i == numChunks) {
                    blocksToPlace = remainder;
                }

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
}

void World::DrawChunks() {
    ChunkRendererSystem& gChunkRendererSystem = ChunkRendererSystem::GetInstance();

    float cameraX = std::floor(mCameraPosition.x/VoxelWorlds::CHUNK_SIZE);
    float cameraY = std::floor(mCameraPosition.y/VoxelWorlds::CHUNK_SIZE);
    float cameraZ = std::floor(mCameraPosition.z/VoxelWorlds::CHUNK_SIZE);

    for (int xChunkPos = -mRenderDistance; xChunkPos <= mRenderDistance; xChunkPos++) {
        for (int zChunkPos = -mRenderDistance; zChunkPos <= mRenderDistance; zChunkPos++) {
            for (int yChunkPos = -mRenderDistance; yChunkPos <= mRenderDistance; yChunkPos++) {
                int coordinatesX = cameraX + xChunkPos;
                int coordinatesY = cameraY + yChunkPos;
                int coordinatesZ = cameraZ + zChunkPos;

                auto chunk = GetChunk(coordinatesX, coordinatesY, coordinatesZ);
                if (!chunk) {
                    continue;
                }
                gChunkRendererSystem.DrawChunk(chunk);
            }
        }
    }
}

void World::WorldVao(int loopX, int loopZ) {
    float cameraX = std::floor(mCameraPosition.x/VoxelWorlds::CHUNK_SIZE);
    float cameraY = std::floor(mCameraPosition.y/VoxelWorlds::CHUNK_SIZE);
    float cameraZ = std::floor(mCameraPosition.z/VoxelWorlds::CHUNK_SIZE);

    int coordinatesX = cameraX + loopX;
    int coordinatesZ = cameraZ + loopZ;

    ChunkManager chunkManger;

    for(int chunkY = -mRenderDistance; chunkY <= mRenderDistance; chunkY++) {
        int coordinatesY = cameraY + chunkY;
        auto chunk = GetChunk(coordinatesX,coordinatesY,coordinatesZ);
        
        if(chunk && chunk->wasGenerated && chunk->mVertexArrayObject == 0 && chunk->mModel.vertexPositions.size() != 0) {
            chunkManger.CreateVAO(*chunk);
        }        
    }
}