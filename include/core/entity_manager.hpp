#pragma once
// C++ standard libraries
#include <unordered_map>
#include <iostream>
#include <typeindex>
#include <memory>
#include <string>
#include <vector>
#include <atomic>
#include <shared_mutex>
#include <mutex>

// Third_party libraries
#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_vector.h>

// Own libraries
#include "component.hpp"
#include "./blocks/block_types.hpp"

using entityIDs = tbb::concurrent_hash_map<std::string, size_t>;
using components = tbb::concurrent_vector<std::shared_ptr<IComponent>>;
using componentsMap = tbb::concurrent_hash_map<std::type_index, tbb::concurrent_vector<std::shared_ptr<IComponent>>>;

class EntityManager {
    public:
        // Outputs if entity creation was successful
        bool CreateEntity(const std::string& entityName);
        void DeleteEntity(const std::string& entityName);
        ~EntityManager();

        template <typename ComponentType>
        void AddComponent(const std::string& entityName) {
            static const std::type_index componentTypeIndex = typeid(ComponentType);

            entityIDs::const_accessor entityAccessor;

            if (!mIDs.find(entityAccessor, entityName)) {
                std::cerr << "Entity with the name: " << entityName << " do not exist. Could not add the component: " << componentTypeIndex.name() << std::endl;
                return;
            }

            componentsMap::accessor componentsAccessor;
            
            mComponents.insert(componentsAccessor, componentTypeIndex);
            
            const size_t& id = entityAccessor->second;
            auto& components = componentsAccessor->second;
            if (id < components.size() && components[id]) {
                std::cerr << "Component exists for " << entityName << std::endl;
                return;
            }

            if (components.size() <= id) {
                components.resize(id + 1);
            }

            components[id] = std::make_shared<ComponentType>();
        }

        template <typename ComponentType>
        void AddComponent(const std::string& entityName, ComponentType& component) {
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            entityIDs::const_accessor entityAccessor;

            if (!mIDs.find(entityAccessor, entityName)) {
                std::cerr << "Entity with the name: " << entityName << " do not exist. Could not add the component: " << componentTypeIndex.name() << std::endl;
                return;
            }

            const size_t& id = entityAccessor->second;
            componentsMap::accessor componentsAccessor;
            mComponents.insert(componentsAccessor, componentTypeIndex);

            auto& components = componentsAccessor->second;
            if (id < components.size() && components[id]) {
                std::cerr << "Component exists for " << entityName << std::endl;
                return;
            }

            if (components.size() <= id) {
                components.resize(id + 1);
            }

            components[id] = std::make_shared<ComponentType>(std::move(component));
        }

        template <typename ComponentType>
        void DeleteComponent(const std::string& entityName) {
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            entityIDs::const_accessor entityAccessor;

            if(!mIDs.find(entityAccessor, entityName)) {
                std::cerr << "Entity with the name: " << entityName << " do not exist. Could not delete the component: " << componentTypeIndex.name() << std::endl;
                return;
            }

            componentsMap::accessor componentsAccessor;
            if(!mComponents.find(componentsAccessor, componentTypeIndex)) {
                return;
            }
            
            const size_t& id = entityAccessor->second;
            auto& components = componentsAccessor->second;
            if (id < components.size()) {
                components[id].reset();
            }
        }

        template <typename ComponentType>
        ComponentType* GetComponent(const std::string& entityName){
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            entityIDs::const_accessor entityAccessor;

            if(!mIDs.find(entityAccessor, entityName)) {
                return nullptr;
            }

            componentsMap::accessor componentsAccessor;
            if (!mComponents.find(componentsAccessor, componentTypeIndex)) {
                return nullptr;
            };
            
            const size_t id = entityAccessor->second;
            auto& components = componentsAccessor->second;
            if (id >= components.size() || !components[id]) {
                return nullptr;
            }
            
            return static_cast<ComponentType*>(components[id].get());
        }

        template <typename ComponentType>
        std::vector<ComponentType*> GetComponentArray() {
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            componentsMap::accessor componentsAccessor;

            std::vector<ComponentType*> components(mNextEntityID, nullptr);
            if(!mComponents.find(componentsAccessor, componentTypeIndex)) {
                return components;
            }

            const auto& componentVector = componentsAccessor->second;
            for(size_t i = 0; i < componentVector.size(); ++i) {
                components[i] = static_cast<ComponentType*>(componentVector[i].get());
            }

            return components;
        }

        tbb::concurrent_hash_map<std::string, size_t>& GetEntities();

        // void InitializeAllComponents();

        static EntityManager& GetInstance();

    private:
        std::atomic<size_t> mNextEntityID = 0;

        tbb::concurrent_hash_map<std::type_index, tbb::concurrent_vector<std::shared_ptr<IComponent>>> mComponents;
        tbb::concurrent_hash_map<std::string, size_t> mIDs;
};