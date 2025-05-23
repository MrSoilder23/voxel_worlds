#pragma once
// C++ standard libraries
#include <memory>
#include <iostream>

#include <typeindex>

// Own libraries
#include "./graphics/shader.hpp"
#include "./core/entity_manager.hpp"
#include "./utility/physics.hpp"

#include "./components/bounding_box_component.hpp"
#include "./components/position_component.hpp"
#include "./components/camera_component.hpp"
#include "./components/model_component.hpp"
#include "./components/chunk_model_component.hpp"
#include "./graphics/graphics.hpp"

class RendererSystem {
    public:
        ~RendererSystem();
        
        void AddGraphicsApp(std::shared_ptr<GraphicsApp> graphicsApp);
    
        void DrawAll(EntityManager& entityManager);
        void DrawAllSingle(EntityManager& entityManager, std::string entityName);

        void DrawAllDebug(EntityManager& entityManager);
        void DrawAllDebugSingle(EntityManager& entityManager, std::string entityName);
        
        static RendererSystem& GetInstance();

    private:
        // RendererSystem();
        // ~RendererSystem();
        // RendererSystem operator=(RendererSystem const& rhs);

    private:
        std::shared_ptr<GraphicsApp> mGraphicsApp;
        
        GLint mModelMatrixLocation;
        GLint mViewLocation;
        GLint mProjectionLocation;
};