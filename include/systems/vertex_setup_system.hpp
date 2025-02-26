#pragma once
// C++ standard libraries
#include <string>

// Third_party libraries
#include <glad/glad.h>

// Own libraries
#include "./core/entity_manager.hpp"
#include "./components/model_component.hpp"
#include "./components/bounding_box_component.hpp"
#include "./core/component.hpp"

class VertexSetupSystem {
    public:
        void CreateVertexSpecification(EntityManager& entityManager);
        void CreateVertexSpecificationSingle(EntityManager& entityManager, std::string entityName);

};