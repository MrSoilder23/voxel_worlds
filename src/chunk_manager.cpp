#include "chunk_manager.hpp"

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

void ChunkManager::InsertToChunk(std::shared_ptr<Chunk>& chunk, BlockData& block, uint8_t x, uint8_t y, uint8_t z) {
    std::shared_ptr<BlockData> blockComponents = std::make_shared<BlockData>(block);
    auto positionComponentLocation = blockComponents->find(std::type_index(typeid(PositionComponent)));
    auto positionComponent = std::dynamic_pointer_cast<PositionComponent>(positionComponentLocation->second);

    positionComponent->SetPosition(x,y,z);

    chunk->blocks.push_back(blockComponents);
}