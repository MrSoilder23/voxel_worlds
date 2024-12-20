#pragma once
// C++ standard libraries
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <string>

// Own libraries
#include "component.hpp"

class EntityManager {
    public:
        void CreateEntity(std::string entityName);
        void DeleteEntity(std::string entityName);
        ~EntityManager();

        template <typename ComponentType>
        void AddComponent(std::string entityName) {
            mEntityComponents[entityName][std::type_index(typeid(ComponentType))] = std::make_shared<ComponentType>();
        }

        template <typename ComponentType>
        void DeleteComponent(std::string entityName) {
            mEntityComponents[entityName].erase(std::type_index(typeid(ComponentType)));
        }

        template <typename ComponentType>
        std::shared_ptr<ComponentType> GetComponent(std::string entityName) {
            return std::dynamic_pointer_cast<ComponentType>(mEntityComponents[entityName][std::type_index(typeid(ComponentType))]);
        }

        std::unordered_map<std::string, std::unordered_map<std::type_index, std::shared_ptr<IComponent>>>& GetEntities();

        void InitializeAllComponents();

        static EntityManager& GetInstance();

    private:
        std::unordered_map<std::string, std::unordered_map<std::type_index, std::shared_ptr<IComponent>>> mEntityComponents;
};