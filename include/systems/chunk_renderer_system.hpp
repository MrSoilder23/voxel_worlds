#pragma once
// C++ standard libraries
#include <memory>
#include <vector>

// Third_party libraries
#include <glm/glm.hpp>

// Own libraries
#include "./graphics/graphics.hpp"
#include "./world/chunk.hpp"
#include "./utility/utility.hpp"

#include "./graphics/model_component.hpp"
#include "./world/position_component.hpp"

class ChunkRendererSystem {
    public:
        void DrawChunk(std::shared_ptr<Chunk>& chunk);

        void AddGraphicsApp(std::shared_ptr<GraphicsApp>& graphicsApp);

        static ChunkRendererSystem& GetInstance();

    private:
        std::shared_ptr<GraphicsApp> mGraphicsApp;
};