#include "./core/entity_manager.hpp"

bool EntityManager::CreateEntity(const std::string& entityName) {
    entityIDs::accessor entityAccessor;

    if (!mIDs.insert(entityAccessor, entityName)) {
        return false;
    }

    entityAccessor->second = mNextEntityID++;

    return true;
}
void EntityManager::DeleteEntity(const std::string& entityName) {
    entityIDs::accessor entityAccessor;
    if (!mIDs.find(entityAccessor, entityName)) {
        return;
    }
    
    mIDs.erase(entityAccessor);
    
    const size_t& id = entityAccessor->second;
    for (auto& [type, components] : mComponents) {
        if (id < components.size()) {
            components[id].reset();
        }
    }

}

tbb::concurrent_hash_map<std::string, size_t>& EntityManager::GetEntities() {
    return mIDs;
}
EntityManager::~EntityManager() {
    std::cout << "EntityManager bye bye" << std::endl;
}

// void EntityManager::InitializeAllComponents() {}

EntityManager& EntityManager::GetInstance() {
    static EntityManager sInstance;
    return sInstance;
}
