#include "./core/entity_manager.hpp"

bool EntityManager::CreateEntity(const std::string& entityName) {
    entityIDs::accessor entityAccessor;
    if (!mIDs.insert(entityAccessor, entityName)) {
        return false;
    }

    entityAccessor->second = mNextEntityID++;

    return true;
}
bool EntityManager::CreateEntity(const glm::ivec3& entityName) {
    chunkEntityIDs::accessor chunkAccessor;
    if (!mChunkSpatialIndex.insert(chunkAccessor, entityName)) {
        return false;
    }

    chunkAccessor->second = mNextEntityID++;
    
    return true;
}

void EntityManager::DeleteEntity(const std::string& entityName) {
    entityIDs::accessor entityAccessor;
    if (!mIDs.find(entityAccessor, entityName)) {
        return;
    }
    
    const size_t& id = entityAccessor->second;
    for (auto& [type, components] : mComponents) {
        if (id < components.size()) {
            components[id].reset();
        }
    }
    
    mIDs.erase(entityAccessor);
}
void EntityManager::DeleteEntity(const glm::ivec3& entityName) {
    chunkEntityIDs::accessor chunkAccessor;
    if (!mChunkSpatialIndex.find(chunkAccessor, entityName)) {
        return;
    }
    
    const size_t& id = chunkAccessor->second;
    for (auto& [type, components] : mComponents) {
        if (id < components.size()) {
            components[id].reset();
        }
    }
    
    mChunkSpatialIndex.erase(chunkAccessor);
}

bool EntityManager::CheckIfChunkEntity(const std::string& entityName, glm::ivec3& coords) {
    int x, y, z;
    if (sscanf(entityName.c_str(), "%d:%d:%d", &x, &y, &z) != 3) {
        return false;
    }
    coords = {x,y,z};
    return true;
}

tbb::concurrent_hash_map<std::string, size_t>& EntityManager::GetEntities() {
    return mIDs;
}
tbb::concurrent_hash_map<glm::ivec3, size_t, Vec3Hash>& EntityManager::GetChunkEntities() {
    return mChunkSpatialIndex;
}
EntityManager::~EntityManager() {
    std::cout << "EntityManager bye bye" << std::endl;
}

// void EntityManager::InitializeAllComponents() {}

EntityManager& EntityManager::GetInstance() {
    static EntityManager sInstance;
    return sInstance;
}
