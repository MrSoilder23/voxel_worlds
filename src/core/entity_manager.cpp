#include "./core/entity_manager.hpp"

bool EntityManager::CreateEntity(const std::string& entityName) {
    std::unique_lock lock(mMutex);
    if(mEntityComponents.find(entityName) == mEntityComponents.end()) {
        mEntityComponents[entityName] = {};
        return true;
    }
    return false;
}
void EntityManager::DeleteEntity(const std::string& entityName) {
    std::unique_lock lock(mMutex);
    mEntityComponents.erase(entityName);
}

std::unordered_map<std::type_index, std::shared_ptr<IComponent>>& EntityManager::GetEntity(const std::string& entityName) {
    return mEntityComponents[entityName];
}

std::unordered_map<std::string, std::unordered_map<std::type_index, std::shared_ptr<IComponent>>>& EntityManager::GetEntities() {
    return mEntityComponents;
}
EntityManager::~EntityManager() {
    std::cout << "EntityManager bye bye" << std::endl;
}

// void EntityManager::InitializeAllComponents() {}

EntityManager& EntityManager::GetInstance() {
    static EntityManager sInstance;
    return sInstance;
}
