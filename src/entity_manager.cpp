#include "entity_manager.hpp"
#include <iostream>

int EntityManager::CreateEntity() {
    mEntityComponents[mNextEntityId] = {};
    return mNextEntityId++;
}
void EntityManager::DeleteEntity(int entityId) {
    mEntityComponents.erase(entityId);
}

std::unordered_map<int, std::unordered_map<std::type_index, std::shared_ptr<IComponent>>>& EntityManager::GetEntities() {
    return mEntityComponents;
}
EntityManager::~EntityManager() {
    std::cout << "EntityManager bye bye" << std::endl;
}

void EntityManager::InitializeAllComponents() {
    for(auto it = mEntityComponents.begin(); it != mEntityComponents.end(); ++it) {
        for(auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            if(jt->second) {
                jt->second->Initialize();
            }
        }
    }
}

EntityManager& EntityManager::GetInstance() {
    static EntityManager sInstance;
    return sInstance;
}
