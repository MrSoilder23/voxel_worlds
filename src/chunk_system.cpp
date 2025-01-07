#include "chunk_system.hpp"

void ChunkSystem::GenerateWorld() {
    ChunkManager& gChunkManager = ChunkManager::GetInstance();
    EntityManager& gEntityManager = EntityManager::GetInstance();

    float cameraX = std::floor(mCameraPosition.x/32.0);
    float cameraZ = std::floor(mCameraPosition.z/32.0);

    auto grass_block = gEntityManager.GetEntity("grass_block");
    auto dirt_block = gEntityManager.GetEntity("dirt_block");

    for (int xChunkPos = cameraX - mRenderDistance; xChunkPos < cameraX + mRenderDistance; xChunkPos++) {
        for (int zChunkPos = cameraZ - mRenderDistance; zChunkPos < cameraZ + mRenderDistance; zChunkPos++) {
            if(gChunkManager.GetChunk(xChunkPos,0,zChunkPos) != nullptr) {
                continue;
            }

            auto chunk = gChunkManager.CreateChunk(xChunkPos,0,zChunkPos);
            utility::MeshTranslate(chunk->GetTransform(), glm::vec3((float)xChunkPos*32.0, 0,(float)zChunkPos*32.0));

            for(float x = 0; x < 32; x++) {
                for(float z = 0; z < 32; z++) {
                    float height = std::round(utility::PerlinNoise(xChunkPos,zChunkPos , x/32.0, z/32.0, mSeed) * 32.0);

                    for(float y = 0; y < height; y++) {
                        if(y == height-1) {
                            gChunkManager.InsertToChunk(chunk, grass_block, x, y, z);
                        } else {
                            gChunkManager.InsertToChunk(chunk, dirt_block, x, y, z);
                            
                        }
                    
                    }
                }
            }
            gChunkManager.InitializeChunk(xChunkPos,0,zChunkPos);
        }
    }
    

}
void ChunkSystem::DrawChunks() {
    ChunkManager& gChunkManager = ChunkManager::GetInstance();
    ChunkRendererSystem& gChunkRendererSystem = ChunkRendererSystem::GetInstance();

    float cameraX = std::floor(mCameraPosition.x/32.0f);
    float cameraZ = std::floor(mCameraPosition.z/32.0f);

    for (int xChunkPos = cameraX - mRenderDistance; xChunkPos < cameraX + mRenderDistance; xChunkPos++) {
        for (int zChunkPos = cameraZ - mRenderDistance; zChunkPos < cameraZ + mRenderDistance; zChunkPos++) {
            auto chunk = gChunkManager.GetChunk((int)xChunkPos, 0, (int)zChunkPos);
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