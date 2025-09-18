#pragma once
// C++ standard libraries
#include <memory>
#include <iostream>

#include <typeindex>

// Own libraries
#include "graphics/shader.hpp"
#include "bismuth/registry.hpp"
#include "utility/physics.hpp"

#include "components/bounding_box_component.hpp"
#include "components/position_component.hpp"
#include "components/camera_component.hpp"
#include "components/core/mesh.hpp"
#include "components/core/material.hpp"
#include "graphics/graphics.hpp"

class RendererSystem {
    public:
        RendererSystem(std::shared_ptr<GraphicsApp> graphicsApp);
        ~RendererSystem();
            
        void update(bismuth::Registry& registry);

    private:
        std::shared_ptr<GraphicsApp> mGraphicsApp;
        
        GLint mModelMatrixLocation;
        GLint mViewLocation;
        GLint mProjectionLocation;
};