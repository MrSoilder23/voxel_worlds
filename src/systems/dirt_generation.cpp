#include "./systems/dirt_generation.hpp"

void DirtGenerationSystem::update(bismuth::Registry& registry) {
    auto chunkView = registry.getView<ChunkStorageComponent, PositionComponent, ChunkStateComponent>();

    std::vector<std::tuple<glm::vec3*, ChunkStorageComponent*, ChunkStateComponent*>> chunks;
    for(auto [entity, storage, position, state] : chunkView) {
        if(state.progress == ChunkProgress::stone_stage) {
            chunks.push_back({&position.position, &storage, &state});
        }
    }

    if(chunks.size() == 0) {
        return;
    }

    std::sort(chunks.begin(), chunks.end(),
        [](const auto& a, const auto& b) {
            const glm::vec3& pa = *std::get<0>(a);
            const glm::vec3& pb = *std::get<0>(b);
    
            if (pa.x != pb.x) return pa.x < pb.x;
            if (pa.z != pb.z) return pa.z < pb.z;
            return pa.y > pb.y;
    });

    std::array<std::queue<BlockTypes*>, VoxelWorlds::CHUNK_SIZE_2D> blocks;
    std::array<uint8_t, VoxelWorlds::CHUNK_SIZE_2D> blocksAir{0};
    
    glm::vec3 currentColumnPos = *std::get<0>(chunks[0]);
    
    for(int i = 0; i < chunks.size(); i++) {
        auto position = std::get<0>(chunks[i]);
        auto storage  = std::get<1>(chunks[i]);
        auto state    = std::get<2>(chunks[i]);

        bool newColumn = (position->x != currentColumnPos.x || position->z != currentColumnPos.z);

        if(newColumn) {
            for(int j = 0; j < blocks.size(); j++) {
                auto& currentColumn = blocks[j];
                auto& currentAir = blocksAir[j];
                
                while(!currentColumn.empty()) {
                    BlockTypes* block = currentColumn.front();
                    currentColumn.pop();

                    if(*block == BlockTypes::stone_block && currentAir > 0) {
                        *block = BlockTypes::dirt_block;
                    } else if(*block == BlockTypes::air) {
                        if(currentAir > 0) {
                            --currentAir;
                        }
                    }
                }
                currentAir = 0;
            }
            
            currentColumnPos = *position;
        }

        for(int y = 0; y < VoxelWorlds::CHUNK_SIZE; y++) {
            for(int z = 0; z < VoxelWorlds::CHUNK_SIZE; z++) {
                for(int x = 0; x < VoxelWorlds::CHUNK_SIZE; x++) {
                    auto index = x + VoxelWorlds::CHUNK_SIZE * z;

                    auto& currentColumn = blocks[index];
                    auto& currentAir    = blocksAir[index];

                    if(currentColumn.size() >= 12) {
                        BlockTypes* block = currentColumn.front();
                        currentColumn.pop();

                        if(*block == BlockTypes::stone_block && currentAir > 0) {
                            *block = BlockTypes::dirt_block;
                        } else if(*block == BlockTypes::air) {
                            if(currentAir > 0) {
                                --currentAir;
                            }
                        }
                    }

                    auto& block = ChunkStorage::getBlock(*storage, x,y,z);
                    currentColumn.push(&block);

                    if(block == BlockTypes::air) {
                        ++currentAir;
                    }
                }
            }    
        }

        state->progress = ChunkProgress::dirt_stage;
        storage->wasGenerated = true;

    }

    for(int j = 0; j < blocks.size(); j++) {
        auto& currentColumn = blocks[j];
        auto& currentAir = blocksAir[j];
        
        while(!currentColumn.empty()) {
            BlockTypes* block = currentColumn.front();
            currentColumn.pop();

            if(*block == BlockTypes::stone_block && currentAir > 0) {
                *block = BlockTypes::dirt_block;
            } else if(*block == BlockTypes::air) {
                if(currentAir > 0) {
                    --currentAir;
                }
            }
        }
    }
}