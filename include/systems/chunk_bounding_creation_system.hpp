#pragma once
// Own libraries
#include "./components/bounding_box_collection_component.hpp"
#include "./components/chunk_storage_component.hpp"
#include "./components/chunk_model_component.hpp"
#include "./core/entity_manager.hpp"

class ChunkBoundingCreationSystem {
    public:
        void CreateChunkBoundingBoxes(int x, int y, int z);

        void SetEntityManager(EntityManager& entityManager);
    
    private:
        EntityManager* mEntityManager;
};