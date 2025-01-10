#include "chunk_system.hpp"

void ChunkSystem::GenerateWorld() {
    ChunkManager& gChunkManager = ChunkManager::GetInstance();
    EntityManager& gEntityManager = EntityManager::GetInstance();

    float cameraX = std::floor(mCameraPosition.x/32.0);
    float cameraZ = std::floor(mCameraPosition.z/32.0);

    static auto& grass_block = gEntityManager.GetEntity("grass_block");
    static auto& dirt_block = gEntityManager.GetEntity("dirt_block");

    static int loopX = 0;
    static int loopZ = 0;
    static int max = 1;
    static bool side = true;


    int coordinatesX = cameraX + loopX;
    int coordinatesZ = cameraZ + loopZ;
    std::cout << "LoopX: " << coordinatesX << " " << "LoopZ: " << coordinatesZ << std::endl;

    if(gChunkManager.GetChunk(coordinatesX,0,coordinatesZ) == nullptr) {
        auto chunk = gChunkManager.CreateChunk(coordinatesX,0,coordinatesZ);
        utility::MeshTranslate(chunk->GetTransform(), glm::vec3(static_cast<float>(coordinatesX)*32.0, 0,static_cast<float>(coordinatesZ)*32.0));
        
        for(float x = 0; x < 32; x++) {
            for(float z = 0; z < 32; z++) {
                float height = std::round(utility::PerlinNoise(coordinatesX,coordinatesZ , x/32.0, z/32.0, mSeed) * 32.0);

                for(float y = 0; y < height; y++) {
                    if(y == height-1) {
                        gChunkManager.InsertToChunk(chunk, grass_block, x, y, z);
                    } else {
                        gChunkManager.InsertToChunk(chunk, dirt_block, x, y, z);
                    }
                }
            }
        }
        // To-Do pre generate chunks and then initialize chunk in the middle
        gChunkManager.InitializeChunk(coordinatesX,0,coordinatesZ);
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

    if(max <= mRenderDistance+1) {
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

void ChunkSystem::GenerateChunk() {
    
}

void ChunkSystem::DrawChunks() {
    ChunkManager& gChunkManager = ChunkManager::GetInstance();
    ChunkRendererSystem& gChunkRendererSystem = ChunkRendererSystem::GetInstance();

    float cameraX = std::floor(mCameraPosition.x/32.0f);
    float cameraZ = std::floor(mCameraPosition.z/32.0f);

    for (int xChunkPos = cameraX - mRenderDistance; xChunkPos < cameraX + mRenderDistance; xChunkPos++) {
        for (int zChunkPos = cameraZ - mRenderDistance; zChunkPos < cameraZ + mRenderDistance; zChunkPos++) {
            auto chunk = gChunkManager.GetChunk(xChunkPos, 0, zChunkPos);
            gChunkRendererSystem.DrawChunk(chunk);
        }
    }
}

void ChunkSystem::SetRenderDistance(int renderDistance) {
    mRenderDistance = renderDistance;
}
void ChunkSystem::SetCameraPosition(glm::vec3 cameraPos) {
    mCameraPosition = cameraPos;
}
void ChunkSystem::SetWorldSeed(unsigned int worldSeed) {
    mSeed = worldSeed;
}