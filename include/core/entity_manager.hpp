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
#include <functional>

// Third_party libraries
#include <tbb/tbb.h>
#include <glm/glm.hpp>

// Own libraries
#include "component.hpp"
#include "./blocks/block_types.hpp"

struct Vec3Hash {
    static size_t hash(const glm::ivec3& key) {
      // Combine hashes of x, y, z using std::hash<int>
      size_t h = 0;
      hash_combine(h, key.x);
      hash_combine(h, key.y);
      hash_combine(h, key.z);
      return h;
    }
  
    static bool equal(const glm::ivec3& a, const glm::ivec3& b) {
      return a == b;
    }
  
  private:
    static void hash_combine(size_t& seed, int value) {
      seed ^= std::hash<int>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};

using entityIDs = tbb::concurrent_hash_map<std::string, size_t>;
using chunkEntityIDs = tbb::concurrent_hash_map<glm::ivec3, size_t, Vec3Hash>;
using components = tbb::concurrent_vector<std::shared_ptr<IComponent>>;
using componentsMap = tbb::concurrent_hash_map<std::type_index, tbb::concurrent_vector<std::shared_ptr<IComponent>>>;

class EntityManager {
    public:
        // Outputs if entity creation was successful
        bool CreateEntity(const std::string& entityName);
        bool CreateEntity(const glm::ivec3& entityName);

        void DeleteEntity(const std::string& entityName);
        void DeleteEntity(const glm::ivec3& entityName);
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
        void AddComponent(const glm::ivec3& entityName) {
            static const std::type_index componentTypeIndex = typeid(ComponentType);

            chunkEntityIDs::const_accessor chunkEntityAccessor;            
            if(!mChunkSpatialIndex.find(chunkEntityAccessor, entityName)) {
                std::cerr << "Entity with the name: " << entityName.x << ":" << entityName.y << ":" << entityName.z 
                << " do not exist. Could not add the component: " << componentTypeIndex.name() << std::endl;
                return;
            }

            componentsMap::accessor componentsAccessor;            
            mComponents.insert(componentsAccessor, componentTypeIndex);
            
            const size_t& id = chunkEntityAccessor->second;
            auto& components = componentsAccessor->second;
            if (id < components.size() && components[id]) {
                std::cerr << "Component exists for " << entityName.x << ":" << entityName.y << ":" << entityName.z << std::endl;
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

            components[id] = std::make_shared<ComponentType>(std::move(component));
        }

        template <typename ComponentType>
        void AddComponent(const glm::ivec3& entityName, ComponentType& component) {
            static const std::type_index componentTypeIndex = typeid(ComponentType);

            chunkEntityIDs::const_accessor chunkEntityAccessor;            
            if(!mChunkSpatialIndex.find(chunkEntityAccessor, entityName)) {
                std::cerr << "Entity with the name: " << entityName.x << ":" << entityName.y << ":" << entityName.z 
                << " do not exist. Could not add the component: " << componentTypeIndex.name() << std::endl;
                return;
            }

            componentsMap::accessor componentsAccessor;
            mComponents.insert(componentsAccessor, componentTypeIndex);
            
            const size_t& id = chunkEntityAccessor->second;
            auto& components = componentsAccessor->second;
            if (id < components.size() && components[id]) {
                std::cerr << "Component exists for " << entityName.x << ":" << entityName.y << ":" << entityName.z << std::endl;
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
        void DeleteComponent(const glm::ivec3& entityName) {
            static const std::type_index componentTypeIndex = typeid(ComponentType);

            chunkEntityIDs::const_accessor chunkEntityAccessor;
            
            if(!mChunkSpatialIndex.find(chunkEntityAccessor, entityName)) {
                std::cerr << "Entity with the name: " << entityName.x << ":" << entityName.y << ":" << entityName.z 
                << " do not exist. Could not delete the component: " << componentTypeIndex.name() << std::endl;
                return;
            }

            componentsMap::accessor componentsAccessor;
            if(!mComponents.find(componentsAccessor, componentTypeIndex)) {
                return;
            }
            
            const size_t& id =chunkEntityAccessor->second;
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
            
            const size_t& id = entityAccessor->second;
            auto& components = componentsAccessor->second;
            if (id >= components.size() || !components[id]) {
                return nullptr;
            }
            
            return static_cast<ComponentType*>(components[id].get());
        }

        template <typename ComponentType>
        ComponentType* GetComponent(const glm::ivec3& entityName){
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            chunkEntityIDs::const_accessor chunkEntityAccessor;
            
            if(!mChunkSpatialIndex.find(chunkEntityAccessor, entityName)) {
                return nullptr;
            }
            
            componentsMap::accessor componentsAccessor;
            if (!mComponents.find(componentsAccessor, componentTypeIndex)) {
                return nullptr;
            };
            
            const size_t& id = chunkEntityAccessor->second;
            auto& components = componentsAccessor->second;
            if (id >= components.size() || !components[id]) {
                return nullptr;
            }
            
            return static_cast<ComponentType*>(components[id].get());
        }

        template <typename ComponentType>
        std::vector<ComponentType*> GetComponentArray() {
            static const std::type_index componentTypeIndex = typeid(ComponentType);
            componentsMap::const_accessor componentsAccessor;

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

        bool CheckIfChunkEntity(const std::string& entityName, glm::ivec3& coords);
        
        tbb::concurrent_hash_map<std::string, size_t>& GetEntities();
        tbb::concurrent_hash_map<glm::ivec3, size_t, Vec3Hash>& GetChunkEntities();

        // void InitializeAllComponents();

        static EntityManager& GetInstance();

    private:
        std::atomic<size_t> mNextEntityID = 0;
        tbb::concurrent_queue<size_t> mEntityIDQueue;

        tbb::concurrent_hash_map<std::type_index, tbb::concurrent_vector<std::shared_ptr<IComponent>>> mComponents;
        tbb::concurrent_hash_map<std::string, size_t> mIDs;
        tbb::concurrent_hash_map<glm::ivec3, size_t, Vec3Hash> mChunkSpatialIndex;
};