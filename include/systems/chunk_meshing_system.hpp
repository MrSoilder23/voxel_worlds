#pragma once
// C++ standard libraries
#include <unordered_map>

// Third_party libraries
#include <glad/glad.h>
#include <tbb/tbb.h>

// Own libraries
#include "./core/entity_manager.hpp"
#include "./components/chunk_model_component.hpp"
#include "./components/chunk_state_component.hpp"
#include "./components/chunk_storage_component.hpp"
#include "./components/bounding_box_collection_component.hpp"
#include "./components/position_component.hpp"
#include "./blocks/block_registry.hpp"
#include "./blocks/block_texture_creator.hpp"
#include "./utility/utility.hpp"

class ChunkMeshingSystem {
    public:
        void CreateChunksMesh(EntityManager& entityManager);
    private:
        std::unordered_map<GLuint64, GLuint> TextureMap();
        
        ChunkStorageComponent* GetNeighbouringChunk(EntityManager& entityManager, const tbb::concurrent_hash_map<glm::ivec3, size_t, Vec3Hash>& entityMap, 
        const std::vector<ChunkStorageComponent*>& storageComponents, int x, int y, int z);

        bool CheckBlock(EntityManager& entityManager, ChunkStorageComponent& currentChunkData, int chunkX, int chunkY, int chunkZ, int x, int y, int z);
};