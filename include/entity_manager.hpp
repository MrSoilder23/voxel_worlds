#pragma once
// C++ standard libraries
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <string>

// Own libraries
#include "component.hpp"
#include "block_types.hpp"

class EntityManager {
    public:
        void CreateEntity(BlockTypes entityName);
        void DeleteEntity(BlockTypes entityName);
        ~EntityManager();

        template <typename ComponentType>
        void AddComponent(BlockTypes entityName) {
            mEntityComponents[entityName][std::type_index(typeid(ComponentType))] = std::make_shared<ComponentType>();
        }

        template <typename ComponentType>
        void DeleteComponent(BlockTypes entityName) {
            mEntityComponents[entityName].erase(std::type_index(typeid(ComponentType)));
        }

        template <typename ComponentType>
        std::shared_ptr<ComponentType> GetComponent(BlockTypes entityName) {
            return std::dynamic_pointer_cast<ComponentType>(mEntityComponents[entityName][std::type_index(typeid(ComponentType))]);
        }

        std::unordered_map<std::type_index, std::shared_ptr<IComponent>>& GetEntity(BlockTypes entityName);

        std::unordered_map<BlockTypes, std::unordered_map<std::type_index, std::shared_ptr<IComponent>>>& GetEntities();

        void InitializeAllComponents();

        static EntityManager& GetInstance();

    private:
        std::unordered_map<BlockTypes, std::unordered_map<std::type_index, std::shared_ptr<IComponent>>> mEntityComponents;
};