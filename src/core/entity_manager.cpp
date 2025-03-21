#include "./core/entity_manager.hpp"

bool EntityManager::CreateEntity(const std::string& entityName) {
    if (mIDs.find(entityName) != mIDs.end()) {
        return false;
    }

    mIDs[entityName] = mNextEntityID++;

    return true;
}
void EntityManager::DeleteEntity(const std::string& entityName) {
    auto entityIt = mIDs.find(entityName);

    if (entityIt == mIDs.end()) return;
    
    size_t id = entityIt->second;
    mIDs.erase(entityIt);
    
    for (auto& [type, components] : mComponents) {
        if (id < components.size()) {
            components[id].reset();
        }
    }

}

std::unordered_map<std::string, size_t>& EntityManager::GetEntities() {
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
