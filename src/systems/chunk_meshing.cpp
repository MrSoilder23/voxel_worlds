#include "systems/chunk_meshing.hpp"

void ChunkMeshingSystem::update(bismuth::Registry& registry) {
    auto chunkView = registry.getView<MeshComponent, PositionComponent, ChunkStorageComponent>();

    entityMap3 chunkMap;
    for(auto [entity, mesh, position, _] : chunkView) {
        glm::ivec3 chunkCoords = position.position / VoxelWorlds::CHUNK_SIZE;
        chunkMap[chunkCoords] = entity;
    }

    // Przelec przez chunka 3 razy, x y z, zdobadz 2 scianki lewa i prawa, pamietaj o sasiadujacych chunkach.
    for(auto [entity, mesh, position, storage] : chunkView) {

        const ChunkStorageComponent* chunkRight = getStorage(registry, chunkMap, position.position.x+1, position.position.y, position.position.z);
        const ChunkStorageComponent* chunkLeft  = getStorage(registry, chunkMap, position.position.x-1, position.position.y, position.position.z);
        const ChunkStorageComponent* chunkTop   = getStorage(registry, chunkMap, position.position.x, position.position.y+1, position.position.z);
        const ChunkStorageComponent* chunkBot   = getStorage(registry, chunkMap, position.position.x, position.position.y-1, position.position.z);
        const ChunkStorageComponent* chunkFront = getStorage(registry, chunkMap, position.position.x, position.position.y, position.position.z+1);
        const ChunkStorageComponent* chunkBack  = getStorage(registry, chunkMap, position.position.x, position.position.y, position.position.z-1);

        // zamien chunk na binarny uzyj uint64 jeden bit odpowiada blokowi 0 air 1 block, pamietaj o paddingu sasiadow, bede potrzebowal 32^2 * 3 array
        std::vector<uint64_t> bitChunk(VoxelWorlds::CHUNK_SIZE_2D * 3);
        
        for(int z = 0; z < VoxelWorlds::CHUNK_SIZE; z++) {
            for(int y = 0; y < VoxelWorlds::CHUNK_SIZE; y++) {
                for(int x = 0; x < VoxelWorlds::CHUNK_SIZE; x++) {
                    
                }
            }
        }

        // 3 Razy (x y z) od boku do boku zrob 2 strony na raz ( lewa i prawa )
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < VoxelWorlds::CHUNK_SIZE+2; j++) {

            }
        }
        
    }
}

inline ChunkStorageComponent* ChunkMeshingSystem::getStorage(
    bismuth::Registry& registry, 
    entityMap3  const& storageComponents,
    int x,
    int y,
    int z
) {
    auto& storagePool = registry.getComponentPool<ChunkStorageComponent>();
    
    glm::ivec3 coords = {x,y,z};
    
    auto it = storageComponents.find(coords);
    if(it == storageComponents.end()) {
        return nullptr;
    }
    
    return &storagePool.getComponent(it->second);

}