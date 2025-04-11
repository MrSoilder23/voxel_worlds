# **VoxelWorlds**

Custom OpenGL engine with infinite terrain, ECS and parallized chunk mesh generation.

&nbsp;
## **Description**

The engine demonstrates inifinitely generating 3D worlds using layered OpenSimplex2 noise, with block interactions, and Entity-Component-System (ECS) architecture. Built from scratch using glad.

&nbsp;
## **Features**

### **_Infinite World Generation_**

- Layered multi noise, using OpenSimplex2 for terrain height.
- Chunk based generation, with parallel mesh generation.
- (Terrain generation performance boost in progress)

### **_Block Interactions_**

- Ray-AABB raycast for block breaking and placing.
- SweptAABB algorithm for block collisions.
- Dynamic chunk updates when modifying terrain.

### **_Engine architecture_**

- Custom ECS for entity management.
- Glad/OpenGL 4.5 rendering using Frustum culling.
- Bindless textures.

### **_Physics_**

- AABB world hitboxes.

&nbsp;
## **Installation**

Dependencies

- Compiler G++ >= 13 Clang >= 16 MSVC >= 19.29 (C++20 required)
- CMake >= 3.3

### Build

```
git clone https://github.com/MrSoilder23/voxel_worlds
cd voxel_worlds
rmdir build
cmake -S . -B build
cmake --build build --config Release
```

### Usage

```
./bin/prog
```

**Controls**
- WASD: Move
- Mouse: Look
- Left Click: Break block
- Right Click: Place current block
- 1-4: Change current block