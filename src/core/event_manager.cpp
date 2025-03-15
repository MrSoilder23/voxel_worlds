#include "./core/event_manager.hpp"

void EventManager::RegisterEvent(InputAction eventName, std::function<void(float)> event) {
    mEvents[eventName].push_back(event);
}
void EventManager::GetEvent(InputAction eventName, float deltaTime) {
    if(mEvents.find(eventName) != mEvents.end()) {
        for(auto& event : mEvents[eventName]) {
            event(deltaTime);
        }
    }
}

void EventManager::RegisterMouseMotionEvent(InputAction eventName, std::function<void(float, int, int)> event) {
    mMouseMotionEvents[eventName].push_back(event);
}
void EventManager::GetMouseMotionEvent(InputAction eventName, float deltaTime, int mouseX, int mouseY) {
    if(mMouseMotionEvents.find(eventName) != mMouseMotionEvents.end()) {
        for(auto& event : mMouseMotionEvents[eventName]) {
            event(deltaTime, mouseX,mouseY);
        }
    }
}

EventManager& EventManager::GetInstance() {
    static EventManager sInstance;
    return sInstance;
}