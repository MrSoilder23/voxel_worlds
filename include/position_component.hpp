#pragma once
// Third_party libraries
#include <glm/glm.hpp>

// Own libraries
#include "component.hpp"

class PositionComponent : public IComponent {
    public:
        void Initialize() override;

        void SetPosition(float _x, float _y, float _z);

        glm::vec3&& GetPosition();
        float& GetPositionX();
        float& GetPositionY();
        float& GetPositionZ();

    private:
        float x,y,z;
};