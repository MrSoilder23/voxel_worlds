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
        ~EntityManager();

        template <typename ComponentType>
        void AddComponent(int entityId) {
            mEntityComponents[entityId][std::type_index(typeid(ComponentType))] = std::make_shared<ComponentType>();
        }

        template <typename ComponentType>
        void DeleteComponent(int entityId) {
            mEntityComponents[entityId].erase(std::type_index(typeid(ComponentType)));
        }

        template <typename ComponentType>
        std::shared_ptr<ComponentType> GetComponent(int entityId) {
            return std::dynamic_pointer_cast<ComponentType>(mEntityComponents[entityId][std::type_index(typeid(ComponentType))]);
        }

        std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<IComponent>>>& GetEntities();

        void InitializeAllComponents();

        static EntityManager& GetInstance();

    private:
        std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<IComponent>>> mEntityComponents;
        
        unsigned int mNextEntityId = 0;
};