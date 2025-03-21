#pragma once
// C++ standard libraries
#include <unordered_map>
#include <iostream>
#include <typeindex>
#include <memory>
#include <string>
#include <vector>

// Own libraries
#include "component.hpp"
#include "./blocks/block_types.hpp"

class EntityManager {
    public:
        // Outputs if entity creation was successful
        bool CreateEntity(const std::string& entityName);
        void DeleteEntity(const std::string& entityName);
        ~EntityManager();

        template <typename ComponentType>
        void AddComponent(const std::string& entityName) {
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            auto entityIt = mIDs.find(entityName);
            if (entityIt == mIDs.end()) {
                std::cerr << "Entity with the name: " << entityName << " do not exist. Could not add the component: " << componentTypeIndex.name() << std::endl;
                return;
            }

            size_t id = entityIt->second;
            auto& components = mComponents[componentTypeIndex];
            if (id < components.size() && components[id]) {
                std::cerr << "Component exists for " << entityName << std::endl;
                return;
            }

            if (components.size() <= id) {
                components.resize(id + 1);
            }

            components[id] = std::make_unique<ComponentType>();
        }

        template <typename ComponentType>
        void AddComponent(const std::string& entityName, ComponentType& component) {
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            auto entityIt = mIDs.find(entityName);

            if (entityIt == mIDs.end()) {
                std::cerr << "Entity with the name: " << entityName << " do not exist. Could not add the component: " << componentTypeIndex.name() << std::endl;
                return;
            }

            size_t id = entityIt->second;
            auto& components = mComponents[componentTypeIndex];
            if (id < components.size() && components[id]) {
                std::cerr << "Component exists for " << entityName << std::endl;
                return;
            }

            if (components.size() <= id) {
                components.resize(id + 1);
            }

            components[id] = std::make_unique<ComponentType>(std::move(component));
        }

        template <typename ComponentType>
        void DeleteComponent(const std::string& entityName) {
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            auto entityIt = mIDs.find(entityName);

            if(entityIt == mIDs.end()) {
                std::cerr << "Entity with the name: " << entityName << " do not exist. Could not delete the component: " << componentTypeIndex.name() << std::endl;
                return;
            }

            size_t id = entityIt->second;
            auto componentIt = mComponents.find(componentTypeIndex);
            if(componentIt == mComponents.end()) {
                return;
            }

            auto& components = componentIt->second;
            if (id < components.size()) {
                components[id].reset();
            }
        }

        template <typename ComponentType>
        ComponentType* GetComponent(const std::string& entityName){
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            auto entityIt = mIDs.find(entityName);

            if(entityIt == mIDs.end()) {
                return nullptr;
            }
            
            size_t id = entityIt->second;
            auto componentIt = mComponents.find(componentTypeIndex);
            if (componentIt == mComponents.end()) {
                return nullptr;
            };

            auto& components = componentIt->second;
            if (id >= components.size() || !components[id]) {
                return nullptr;
            }
            
            return static_cast<ComponentType*>(components[id].get());
        }

        template <typename ComponentType>
        std::vector<ComponentType*> GetComponentArray() {
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            auto componentIt = mComponents.find(componentTypeIndex);

            std::vector<ComponentType*> components(mNextEntityID, nullptr);
            if(componentIt == mComponents.end()) {
                return components;
            }

            const auto& componentVector = componentIt->second;
            for(size_t i = 0; i < componentVector.size(); ++i) {
                components[i] = static_cast<ComponentType*>(componentVector[i].get());
            }

            return components;
        }

        std::unordered_map<std::string, size_t>& GetEntities();

        // void InitializeAllComponents();

        static EntityManager& GetInstance();

    private:
        size_t mNextEntityID = 0;

        std::unordered_map<std::type_index, std::vector<std::unique_ptr<IComponent>>> mComponents;
        std::unordered_map<std::string, size_t> mIDs;
};