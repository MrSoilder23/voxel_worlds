#include "chunk_manager.hpp"
#include <iostream>

ChunkManager::~ChunkManager() {
    std::cout << "Chunk manager bye bye" << std::endl;
}

std::shared_ptr<Chunk>& ChunkManager::CreateChunk(int x, int y, int z) {
    ChunkKey key = std::make_tuple(x,y,z);
    if (!chunks.contains(key)) {
        chunks[key] = std::make_shared<Chunk>();
    }
    return chunks[key];
}
        
std::shared_ptr<Chunk> ChunkManager::GetChunk(int x, int y, int z) {
    ChunkKey key = std::make_tuple(x,y,z);
    if(chunks.contains(key)) {
        return chunks[key];
    }
    return nullptr;
}

void ChunkManager::InsertToChunk(std::shared_ptr<Chunk>& chunk, BlockData& block, int x, int y, int z) {
    BlockData blockComponents;

    for(const auto& [type, component] : block) {
        
        if(type == std::type_index(typeid(PositionComponent))) {
            auto positionComponent = std::dynamic_pointer_cast<PositionComponent>(component);
            blockComponents[type] = std::make_shared<PositionComponent>(*positionComponent);
            
        } else if(type == std::type_index(typeid(ModelComponent))) {
            auto modelComponent = std::dynamic_pointer_cast<ModelComponent>(component);
            blockComponents[type] = std::make_shared<ModelComponent>(*modelComponent);

        } else {
            blockComponents[type] = component;
        }

    }

    auto positionComponentLocation = blockComponents.find(std::type_index(typeid(PositionComponent)));
    auto modelComponentLocation = blockComponents.find(std::type_index(typeid(ModelComponent)));

    auto positionComponent = std::dynamic_pointer_cast<PositionComponent>(positionComponentLocation->second);
    auto modelComponent = std::dynamic_pointer_cast<ModelComponent>(modelComponentLocation->second);

    positionComponent->SetPosition((float)x,(float)y,(float)z);

    utility::MeshTranslate(modelComponent->GetTransform(), positionComponent->GetPosition());

    chunk->blocks[x][y][z] = blockComponents;
}

void ChunkManager::InitializeChunk(int x, int y, int z) {
    ChunkKey key = std::make_tuple(x,y,z);

    if (chunks.find(key) == chunks.end()) {
        return;
    }

    auto chunk = chunks[key];

    for(int blockX = 0; blockX < 32; blockX++) {
        for (int blockY = 0; blockY < 32; blockY++) {
            for (int blockZ = 0; blockZ < 32; blockZ++) {
                
                auto modelComponentLocation = chunk->blocks[blockX][blockY][blockZ].find(std::type_index(typeid(ModelComponent)));
                if(modelComponentLocation == chunk->blocks[blockX][blockY][blockZ].end()) {
                    continue;
                }
                auto modelComponent = std::dynamic_pointer_cast<ModelComponent>(modelComponentLocation->second);

                std::vector<GLuint> indexes;

                if(GetBlock(x,y,z ,blockX+1, blockY, blockZ).empty()) {
                    indexes.insert(indexes.end(), {3,1,7, 5,7,1}); // RightFace
                }
                if(GetBlock(x,y,z ,blockX-1, blockY, blockZ).empty()) {
                    indexes.insert(indexes.end(), {6,4,2, 0,2,4}); // LeftFace
                }
                if(GetBlock(x,y,z ,blockX, blockY+1, blockZ).empty()) {
                    indexes.insert(indexes.end(), {6,2,7, 3,7,2}); // TopFace
                }
                if(GetBlock(x,y,z ,blockX, blockY-1, blockZ).empty()) {
                    indexes.insert(indexes.end(), {0,4,5, 1,0,5}); // BotFace
                }
                if(GetBlock(x,y,z ,blockX, blockY, blockZ+1).empty()) {
                    indexes.insert(indexes.end(), {0,1,2, 3,2,1}); // FrontFace
                }
                if(GetBlock(x,y,z ,blockX, blockY, blockZ-1).empty()) {
                    indexes.insert(indexes.end(), {6,7,4, 7,5,4}); // BackFace
                }

                modelComponent->GetModel().indexBufferData.insert(modelComponent->GetModel().indexBufferData.end(), indexes.begin(), indexes.end());
                indexes.clear();
            }
        }
    }

    chunk->CreateVao();
}

BlockData ChunkManager::GetBlock(int chunkX, int chunkY, int chunkZ, int x, int y, int z) {
    if(x < 0) {
        x = 31;
        chunkX = chunkX-1;
    } else if(x > 31) {
        x = 0;
        chunkX = chunkX+1;
    } else if(y < 0) {
        y = 31;
        chunkY = chunkY-1;
    } else if(y > 31) {
        y = 0;
        chunkY = chunkY+1;
    } else if(z < 0) {
        z = 31;
        chunkZ = chunkZ-1;
    } else if(z > 31) {
        z = 0;
        chunkZ = chunkZ+1;
    }
    ChunkKey key = std::make_tuple(chunkX,chunkY,chunkZ);

    if(chunks.find(key) == chunks.end()) {
        return {}; 
    }

    return chunks[key]->blocks[x][y][z];
}

ChunkManager& ChunkManager::GetInstance() {
    static ChunkManager sInstance;
    return sInstance;
}