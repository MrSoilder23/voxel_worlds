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

void ChunkManager::InsertToChunk(std::shared_ptr<Chunk>& chunk, BlockData& block, float x, float y, float z) {
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

    positionComponent->SetPosition(x,y,z);
    utility::MeshTranslate(modelComponent->GetMeshData(), positionComponent->GetPosition());

    chunk->blocks.push_back(blockComponents);
}

ChunkManager& ChunkManager::GetInstance() {
    static ChunkManager sInstance;
    return sInstance;
}