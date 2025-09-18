#pragma once
// C++ standard libraries
#include <array>
#include <stdexcept>

// Third_party libraries
#include <glm/glm.hpp>

// Own libraries
#include "block_types.hpp"
#include "model.hpp"
#include "components/core/mesh.hpp"
#include "components/core/material.hpp"

class BlockRegistry {
    public:
        static constexpr size_t NUM_BLOCK_TYPES = static_cast<size_t>(BlockTypes::COUNT);

        void registerBlock(
            BlockTypes    type,
            MeshComponent mesh
        );
        const MeshComponent& getMesh(BlockTypes type) const;

        static MeshComponent createCubeMesh(const std::array<glm::vec2, 6>& faceTexCoords);

        static BlockRegistry& getInstance();
    private: 
        std::array<MeshComponent, NUM_BLOCK_TYPES> mMesh;

};