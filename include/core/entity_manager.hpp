#pragma once
// C++ standard libraries
#include <unordered_map>
#include <iostream>
#include <typeindex>
#include <memory>
#include <string>
#include <shared_mutex>
#include <mutex>

// Own libraries
#include "component.hpp"
#include "./blocks/block_types.hpp"

using Entity = std::unordered_map<std::type_index, std::shared_ptr<IComponent>>;

class EntityManager {
    public:
        // Outputs if entity creation was successful
        bool CreateEntity(const std::string& entityName);
        void DeleteEntity(const std::string& entityName);
        ~EntityManager();

        template <typename ComponentType>
        void AddComponent(const std::string& entityName) {
            std::unique_lock lock(mMutex);
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            auto entityIt = mEntityComponents.find(entityName);

            if(entityIt == mEntityComponents.end()) {
                std::cerr << "Entity with the name: " << entityName << " do not exist. Could not add the component: " << componentTypeIndex.name() << std::endl;
                return;
            }

            auto& componentMap = entityIt->second;
            if (componentMap.find(componentTypeIndex) != componentMap.end()) {
                std::cerr << "The component: " << componentTypeIndex.name() << " already exists in this entity: " << entityName << std::endl;
                return;
            }

            componentMap[componentTypeIndex] = std::make_shared<ComponentType>();
        }

        template <typename ComponentType>
        void DeleteComponent(const std::string& entityName) {
            std::unique_lock lock(mMutex);
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            auto entityIt = mEntityComponents.find(entityName);

            if(entityIt == mEntityComponents.end()) {
                std::cerr << "Entity with the name: " << entityName << " do not exist. Could not delete the component: " << componentTypeIndex.name() << std::endl;
                return;
            }

            auto& componentMap = entityIt->second;
            componentMap.erase(componentTypeIndex);
        }

        template <typename ComponentType>
        std::shared_ptr<ComponentType> GetComponent(const std::string& entityName) {
            std::unique_lock lock(mMutex);
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            auto entityIt = mEntityComponents.find(entityName);

            if(entityIt == mEntityComponents.end()) {
                return nullptr;
            }

            const auto& componentMap = entityIt->second;
            auto componentIt = componentMap.find(componentTypeIndex);

            if (componentIt == componentMap.end()) {
                return nullptr;
            }

            return std::static_pointer_cast<ComponentType>(componentIt->second);
        }

        Entity& GetEntity(const std::string& entityName);

        std::unordered_map<std::string, Entity>& GetEntities();

        // void InitializeAllComponents();

        static EntityManager& GetInstance();

    private:
        std::unordered_map<std::string, Entity> mEntityComponents;
        std::shared_mutex mMutex;
};