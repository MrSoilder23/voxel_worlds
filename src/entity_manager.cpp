#include "entity_manager.hpp"

int EntityManager::CreateEntity() {
    return mNextEntityId++;
}
void EntityManager::DeleteEntity(int entityId) {
    mEntityComponents.erase(entityId);
}

template <typename ComponentType>
void EntityManager::AddComponent(int entityId) {
    std::type_index index = std::type_index(typeid(ComponentType));
    mEntityComponents[entityId][index] = std::make_unique<ComponentType>();
}

template <typename ComponentType>
void EntityManager::DeleteComponent(int entityId) {
    mEntityComponents[entityId].erase(ComponentType)
}

template <typename ComponentType>
ComponentType& EntityManager::GetComponent(int entityId) {
    return mEntityComponents[entityId][ComponentType];
}

EntityManager& EntityManager::GetInstance() {
    static EntityManager sInstance;
    return sInstance;
}
