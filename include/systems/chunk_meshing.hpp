#pragma once
// C++ Standard libraries
#include <bit>

// Own libraries
#include "bismuth/registry.hpp"
#include "components/core/mesh.hpp"
#include "components/core/material.hpp"
#include "components/position_component.hpp"
#include "components/chunk_storage_component.hpp"
#include "components/bounding_box_collection_component.hpp"
#include "components/chunk_state_component.hpp"
#include "blocks/block_registry.hpp"
#include "blocks/block_texture_creator.hpp"
#include "utility/utility.hpp"
#include "utility/ivec3_hash.hpp"

class ChunkMeshingSystem {
    public:
        void update(bismuth::Registry& registry);

    private:
        using entityMap3 = std::unordered_map<glm::ivec3, bismuth::EntityID, IVec3Hash>;

        struct NeighboringChunks {
            ChunkStorageComponent* center= nullptr;
            ChunkStorageComponent* right = nullptr;
            ChunkStorageComponent* left  = nullptr;
            ChunkStorageComponent* top   = nullptr;
            ChunkStorageComponent* bot   = nullptr;
            ChunkStorageComponent* front = nullptr;
            ChunkStorageComponent* back  = nullptr;
        };

        void addFace(
            MeshComponent  & chunkModel,
            BlockTypes     & blockType,
            glm::ivec3 const blockPos,
            int axis
        );

        ChunkStorageComponent* getStorage(
            bismuth::Registry& registry, 
            entityMap3  const& storageComponents,
            int x,
            int y,
            int z
        );

        // If solid block True if Air False
        bool checkBlock(
            NeighboringChunks const& chunks,
            glm::ivec3        const& localBlockPos
        );
};