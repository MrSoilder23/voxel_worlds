#include "entity_manager.hpp"
#include <iostream>

void EntityManager::CreateEntity(int entityName) {
    mEntityComponents[entityName] = {};
}
void EntityManager::DeleteEntity(int entityName) {
    mEntityComponents.erase(entityName);
}

std::unordered_map<std::type_index, std::shared_ptr<IComponent>>& EntityManager::GetEntity(int entityName) {
    return mEntityComponents[entityName];
}

std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<IComponent>>>& EntityManager::GetEntities() {
    return mEntityComponents;
}
EntityManager::~EntityManager() {
    std::cout << "EntityManager bye bye" << std::endl;
}

void EntityManager::InitializeAllComponents() {

}

EntityManager& EntityManager::GetInstance() {
    static EntityManager sInstance;
    return sInstance;
}
