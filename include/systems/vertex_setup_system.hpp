#pragma once
// Third_party libraries
#include <glad/glad.h>
// Own libraries
#include "./core/entity_manager.hpp"
#include "./components/model_component.hpp"

class VertexSetupSystem {
    public:
        void CreateVertexSpecification(EntityManager& entityManager);

};