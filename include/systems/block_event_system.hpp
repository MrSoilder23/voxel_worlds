#pragma once
// Own libraries
#include "bismuth/registry.hpp"
#include "components/block_break_event_component.hpp"
#include "components/block_place_event_component.hpp"
#include "components/block_event_component.hpp"
#include "components/chunk_state_component.hpp"
#include "components/chunk_storage_component.hpp"
#include "components/position_component.hpp"
#include "components/chunk_tag.hpp"
#include "utility/ivec3_hash.hpp"

class BlockEventSystem {
    public:
        void update(bismuth::Registry& registry);
};