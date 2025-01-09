#pragma once
// C++ standard libraries
#include <thread>

// Third_party libraries
#include <glm/glm.hpp>

// Own libraries
#include "chunk_manager.hpp"
#include "entity_manager.hpp"
#include "chunk_renderer_system.hpp"
#include "utility.hpp"

class ChunkSystem {
    public:
        void GenerateWorld();
        void GenerateChunk();
        void DrawChunks();

        void SetRenderDistance(int renderDistance);
        void SetCameraPosition(glm::vec3 cameraPos);
        void SetWorldSeed(unsigned int worldSeed);

    private:
        glm::vec3 mCameraPosition;
        unsigned int mSeed;
        float mRenderDistance;
};