# VoxelWorlds <img alt="Bob" src="https://github.com/MrSoilder23/voxel_worlds/blob/main/readme_assets/image2.png" height="25px" />

Custom OpenGL engine with infinite terrain, ECS and parallized chunk mesh generation.

&nbsp;
## **Description**

The engine demonstrates inifinitely generating 3D worlds using layered OpenSimplex2 noise, with block interactions, and Entity-Component-System (ECS) architecture. Built from scratch using glad.

![Image of mountains](https://github.com/MrSoilder23/voxel_worlds/blob/main/readme_assets/image.png)

<p float="left">
  <img alt="Image of Bob from different perspective" src="https://github.com/MrSoilder23/voxel_worlds/blob/main/readme_assets/image3.png" width="49.7%" /> 
  <img alt="Image of Bob" src="https://github.com/MrSoilder23/voxel_worlds/blob/main/readme_assets/image2.png" width="49.7%" />
</p>

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

### **_Physics_**

- AABB world hitboxes.

&nbsp;
## **Installation**

Dependencies

- Compiler G++ >= 13 Clang >= 16 MSVC >= 19.29 (C++20 required)
- CMake >= 3.3

### Build

```
git clone --recurse-submodules https://github.com/MrSoilder23/voxel_worlds.git
cd voxel_worlds
```

For Linux
```
./vcpkg/bootstrap-vcpkg.sh
```

For Windows
```
vcpkg/bootstrap-vcpkg.bat
```

```
cmake -S . -B build
cmake --build build --config Release
```

### Usage

```
./bin/Release/prog
```

**Controls**
- WASD: Move
- Shift: fly down
- Space: fly Up
- Mouse: Look
- Left Click: Break block
- Right Click: Place current block
- 1-5: Change current block
  
- F10: enables/disables world generation
- F11: enables/disables collisions
- F12: enables/disables debug bounding boxes
- ESC: leave game
