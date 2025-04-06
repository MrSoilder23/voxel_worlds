#pragma once
// Own libraries
#include "./core/entity_manager.hpp"
#include "./components/block_break_event_component.hpp"
#include "./components/block_place_event_component.hpp"
#include "./components/block_event_component.hpp"
#include "./components/chunk_state_component.hpp"
#include "./components/chunk_storage_component.hpp"

class BlockEventSystem {
    public:
        void UpdateChunks(EntityManager& entityManager);
};