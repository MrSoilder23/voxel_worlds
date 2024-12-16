#pragma once
// C++ standard libraries
#include <memory>
#include <typeindex>

// Own libraries
#include "graphics.hpp"
#include "model_component.hpp"
#include "entity_manager.hpp"

class RendererSystem {
    public:
        void DrawAll();

        void AddGraphicsApp(std::shared_ptr<GraphicsApp> graphicsApp);

        static RendererSystem& GetInstance();

    private:
        RendererSystem();
        ~RendererSystem();
        RendererSystem operator=(RendererSystem const& rhs);

    private:
        std::shared_ptr<GraphicsApp> mGraphicsApp;
};