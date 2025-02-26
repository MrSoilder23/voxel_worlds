#pragma once
// C++ standard libraries
#include <algorithm>
#include <map>
#include <string>

// Third_party libraries
#include <glad/glad.h>

// Own libraries
#include "./core/entity_manager.hpp"
#include "./components/chunk_model_component.hpp"
#include "./blocks/block_texture_creator.hpp"

class ChunkVertexSetupSystem {
    public:
        void CreateVertexSpecification(EntityManager& entityManager);
        void CreateVertexSpecificationSingle(EntityManager& entityManager, std::string entityName);
};