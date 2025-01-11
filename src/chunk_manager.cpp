#include "chunk_manager.hpp"

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

    static const std::type_index positionType = std::type_index(typeid(PositionComponent));
    static const std::type_index modelType = std::type_index(typeid(ModelComponent));

    for(const auto& [type, component] : block) {
        if(type == positionType) {
            blockComponents.emplace(type, std::make_shared<PositionComponent>(
                                    *std::static_pointer_cast<PositionComponent>(component)));
        } else if(type == modelType) {
            blockComponents.emplace(type, std::make_shared<ModelComponent>(
                                    *std::static_pointer_cast<ModelComponent>(component)));
        } else {
            blockComponents.emplace(type, component);
        }
    }

    auto positionComponentLocation = blockComponents.find(positionType);
    auto modelComponentLocation = blockComponents.find(modelType);

    auto positionComponent = std::static_pointer_cast<PositionComponent>(positionComponentLocation->second);
    auto modelComponent = std::static_pointer_cast<ModelComponent>(modelComponentLocation->second);

    positionComponent->x = static_cast<float>(x);
    positionComponent->y = static_cast<float>(y);
    positionComponent->z = static_cast<float>(z);
    
    Transform& transform = modelComponent->mTransform;
    Model& model = modelComponent->mModel;

    utility::MeshTranslate(transform, static_cast<float>(x),
                                      static_cast<float>(y),
                                      static_cast<float>(z));

    for (size_t i = 0; i < model.vertexPositions.size(); i++) {
        model.vertexPositions[i] = glm::vec3(transform.mModelMatrix * glm::vec4(model.vertexPositions[i], 1.0f));
    }
    
    chunk->blocks[x][y][z] = std::move(blockComponents);
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
                auto modelComponent = std::static_pointer_cast<ModelComponent>(modelComponentLocation->second);

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

                modelComponent->mModel.indexBufferData = std::move(indexes);
            }
        }
    }

    chunk->CreateVao();
}

BlockData ChunkManager::GetBlock(int chunkX, int chunkY, int chunkZ, int x, int y, int z) {
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
        return {}; 
    }

    return chunks[key]->blocks[x][y][z];
}

ChunkManager& ChunkManager::GetInstance() {
    static ChunkManager sInstance;
    return sInstance;
}