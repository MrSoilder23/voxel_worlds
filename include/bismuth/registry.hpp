#pragma once
// C++ standard libraries
#include <bit>
#include <cstddef>
#include <memory>
#include <vector>
#include <array>
#include <cstdint>
#include <typeindex>
#include <unordered_map>

// Own libraries
#include "bismuth/storage/component_pool.hpp"
#include "bismuth/storage/component_view.hpp"

namespace bismuth {

namespace internal_id_gen {
    inline size_t id = 0;
    
    template<typename>
    size_t generate_id() {
        static size_t componentID = id++;
        return componentID;
    }
}

class Registry {
    public:
        template<typename ComponentName>
        inline size_t getPoolID() const {
            static size_t componentID = internal_id_gen::generate_id<ComponentName>();
            return componentID;
        }

        template<typename ComponentName>
        ComponentPool<ComponentName>& getComponentPool() {
            static const size_t type_id = getPoolID<ComponentName>();

            if (type_id >= mComponentPool.size()) {
                mComponentPool.resize(type_id + 1);
            }
            
            if (!mComponentPool[type_id]) {
                mComponentPool[type_id] = std::make_unique<ComponentPool<ComponentName>>();
            }
            
            return *static_cast<ComponentPool<ComponentName>*>(mComponentPool[type_id].get());
        }

        uint32_t createEntity() {
            const uint32_t id = mEntities.size();
            mEntities.emplace_back();
            return id;
        }

        template<typename ComponentName, typename... Args>
        void emplaceComponent(size_t entityID, Args&&... args) {
            assert(entityID < mEntities.size() && "Invalid entity ID");
            
            auto& pool = getComponentPool<ComponentName>();
            const size_t compID = getPoolID<ComponentName>();
            
            pool.addComponent(entityID, std::forward<Args>(args)...);
            mEntities[entityID] |= (1ULL << compID);
        }

        template<typename ComponentName>
        bool hasComponent(size_t entityID) const {
            if (entityID >= mEntities.size()) return false;
            const size_t compID = getPoolID<ComponentName>();
            return (mEntities[entityID] >> compID) & 1;
        }

        template<typename ComponentName>
        void removeComponent(size_t entityID) {
            if (entityID >= mEntities.size()) return;
            
            auto& pool = getComponentPool<ComponentName>();
            const size_t compID = getPoolID<ComponentName>();
            
            pool.removeComponent(entityID);
            mEntities[entityID] &= ~(1ULL << compID);
        }

        void removeEntity(size_t entityID) {
            if (entityID >= mEntities.size()) return;
            
            uint64_t mask = mEntities[entityID];
            while (mask) {
                const size_t idx = std::countr_zero(mask);
                mComponentPool[idx]->removeComponent(entityID);
                mask &= ~(1ULL << idx);
            }
            mEntities[entityID] = 0;
        }

        // Singleton
        template<typename ComponentName>
        ComponentName& getSingleton() {
            static std::type_index typeIndex = std::type_index(typeid(ComponentName));
            auto it = mSingletons.find(typeIndex);
            assert(it != mSingletons.end() && "Singleton not found");

            return *static_cast<ComponentName*>(it->second.get());
        }

        template<typename ComponentName, typename... Args>
        void emplaceSingleton(Args&&... args) {
            static std::type_index typeIndex = std::type_index(typeid(ComponentName));
            assert(mSingletons.find(typeIndex) == mSingletons.end() && "Singleton of this type already exists");

            auto ptr = std::make_shared<ComponentName>(std::forward<Args>(args)...);
            mSingletons[typeIndex] = ptr;
        }

        template<typename ComponentName>
        bool hasSingleton() const {
            static std::type_index typeIndex = std::type_index(typeid(ComponentName));
            return mSingletons.contains(typeIndex);
        }

        template<typename ComponentName>
        void removeSingleton() {
            static std::type_index typeIndex = std::type_index(typeid(ComponentName));
            mSingletons.erase(typeIndex);
        }

        template<typename... ComponentName>
        ComponentView<ComponentName...> getView() {
            return ComponentView<ComponentName...>(getComponentPool<ComponentName>()...);
        }

    private:
        std::unordered_map<std::type_index, std::shared_ptr<void>> mSingletons;
        
        std::vector<uint64_t> mEntities; // Component bitmask per entity
        std::vector<std::unique_ptr<ISparseSet>> mComponentPool;
};

}