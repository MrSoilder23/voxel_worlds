#pragma once
// C++ standard libraries
#include <unordered_map>
#include <typeindex>
#include <memory>

// Own libraries
#include "component.hpp"

class EntityManager {
    public:
        int CreateEntity();
        void DeleteEntity(int entityId);

        template <typename ComponentType>
        void AddComponent(int entityId);

        template <typename ComponentType>
        void DeleteComponent(int entityId);

        template <typename ComponentType>
        ComponentType& GetComponent(int entityId);

        static EntityManager& GetInstance();

    private:
        std::unordered_map<int, std::unordered_map<std::type_index, std::unique_ptr<IComponent>>> mEntityComponents;
        
        unsigned int mNextEntityId = 0;
};