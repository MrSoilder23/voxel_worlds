#pragma once
// Own libraries
#include "./core/component.hpp"
#include "./chunk_progress.hpp"

struct ChunkStateComponent : public IComponent {
    ChunkProgress mProgress;
};