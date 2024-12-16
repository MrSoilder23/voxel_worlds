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
    mEntityComponents[entityId].erase(std::type_index(typeid(ComponentType)));
}

template <typename ComponentType>
ComponentType& EntityManager::GetComponent(int entityId) {
    return mEntityComponents[entityId][std::type_index(typeid(ComponentType))];
}

std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<IComponent>>>& EntityManager::GetEntities() {
    return mEntityComponents;
}

EntityManager& EntityManager::GetInstance() {
    static EntityManager sInstance;
    return sInstance;
}
