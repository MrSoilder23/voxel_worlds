#pragma once

enum class ChunkProgress {
    pending,
    stone_stage,
    dirt_stage,
    partially_generated,
    fully_generated
};