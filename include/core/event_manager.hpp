#pragma once
// C++ standard libraries
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>

// Own libraries
#include "./core/inputs.hpp"

class EventManager {
    public:
        void RegisterEvent(InputAction eventName, std::function<void(float)> event);
        void GetEvent(InputAction eventName, float deltaTime);

        void RegisterMouseMotionEvent(InputAction eventName, std::function<void(float, int, int)> event);
        void GetMouseMotionEvent(InputAction eventName, float deltaTime, int mouseX, int mouseY);

        static EventManager& GetInstance();
    private:
        std::unordered_map<InputAction, std::vector<std::function<void(float)>>> mEvents;
        std::unordered_map<InputAction, std::vector<std::function<void(float, int, int)>>> mMouseMotionEvents;
};