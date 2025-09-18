#pragma once
// C++ standard libraries
#include <unordered_map>

// Third_party libraries
#include <glad/glad.h>
#include <tbb/tbb.h>

// Own libraries
#include "bismuth/registry.hpp"
#include "components/chunk_state_component.hpp"
#include "components/chunk_storage_component.hpp"
#include "components/bounding_box_collection_component.hpp"
#include "components/position_component.hpp"
#include "components/core/mesh.hpp"
#include "components/core/material.hpp"
#include "blocks/block_registry.hpp"
#include "blocks/block_texture_creator.hpp"
#include "utility/utility.hpp"
#include "utility/ivec3_hash.hpp"

class ChunkMeshingSystem {
    public:
        void update(bismuth::Registry& registry);

    private:
        using entityMap3 = std::unordered_map<glm::ivec3, bismuth::EntityID, IVec3Hash>;

        const ChunkStorageComponent* getNeighbouringChunk(
            bismuth::Registry & registry,
            entityMap3   const& storageComponents,
            int x,
            int y,
            int z
        );

        bool checkBlock(
            bismuth::ComponentPool<ChunkStorageComponent>& chunkStoragePool,
            entityMap3      const& entityMap,
            ChunkStorageComponent& currentChunkData,
            int chunkX,
            int chunkY,
            int chunkZ,
            int x,
            int y,
            int z
        );

        void fillBlockSide(
            MeshComponent const& blockObject,
            MeshComponent      & chunkModel,
            int           const  textureSide,
            int           const  vertexOffset, 
            GLuint        const& baseOffset, 
            glm::vec3     const& chunkOffset,
            GLuint a = 2, 
            GLuint b = 0, 
            GLuint c = 1,
            GLuint d = 3 
        );

        void rightBoundary(
            ChunkStorageComponent const*& targetChunk,
            int                        & target,
            ChunkStorageComponent const* currentStorage,
            ChunkStorageComponent const* neigbourStorage
        );
        void leftBoundary(
            ChunkStorageComponent const*& targetChunk,
            int                        & target,
            ChunkStorageComponent const* currentStorage,
            ChunkStorageComponent const* neigbourStorage
        );

        void getBlockNeighbours(
            ChunkStorageComponent const* storage,
            MeshComponent              & chunkModel,
            BlockTypes            const& block,
            GLuint                const  vertexSize,
            ChunkStorageComponent const* chunkRight,
            ChunkStorageComponent const* chunkLeft,
            ChunkStorageComponent const* chunkTop,
            ChunkStorageComponent const* chunkBot,
            ChunkStorageComponent const* chunkFront,
            ChunkStorageComponent const* chunkBack,
            int                   const  blockX,
            int                   const  blockY,
            int                   const  blockZ
        );
};