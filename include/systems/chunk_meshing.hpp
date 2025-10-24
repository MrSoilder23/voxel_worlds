#pragma once
// Own libraries
#include "bismuth/registry.hpp"
#include "components/core/mesh.hpp"
#include "components/position_component.hpp"
#include "components/chunk_storage_component.hpp"
#include "utility/utility.hpp"
#include "utility/ivec3_hash.hpp"

class ChunkMeshingSystem {
    public:
        void update(bismuth::Registry& registry);

    private:
        using entityMap3 = std::unordered_map<glm::ivec3, bismuth::EntityID, IVec3Hash>;

        ChunkStorageComponent* getStorage(
            bismuth::Registry& registry, 
            entityMap3  const& storageComponents,
            int x,
            int y,
            int z
        );
};