#pragma once
// C++ standard libraries
#include <string>
#include <memory>
#include <random>

// Third_party libraries
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tbb/tbb.h>

// Own libraries
#include "bismuth/registry.hpp"
#include "core/game.hpp"
#include "core/event_manager.hpp"
#include "utility/settings.hpp"
#include "utility/thread_pool.hpp"
#include "utility/circle_loop.hpp"
#include "graphics/graphics.hpp"

#include "blocks/block_texture_creator.hpp"
#include "blocks/block_types.hpp"
#include "blocks/block_registry.hpp"

#include "systems/renderer_system.hpp"
#include "systems/player_controller_system.hpp"
#include "systems/vertex_setup_system.hpp"
#include "systems/bounding_box_system.hpp"
#include "systems/position_update_system.hpp"
#include "systems/world_generation_system.hpp"
#include "systems/collision_system.hpp"
#include "systems/physics_system.hpp"
#include "systems/chunk_meshing.hpp"
#include "systems/block_event_system.hpp"
#include "systems/chunk_unload_system.hpp"
#include "systems/chunk_creation_system.hpp"
#include "systems/texture_manager.hpp"

#include "components/player_controller_component.hpp"
#include "components/position_component.hpp"
#include "components/camera_component.hpp"
#include "components/bounding_box_component.hpp"
#include "components/physics_component.hpp"
#include "components/inventory_component.hpp"
#include "components/position_component.hpp"
#include "components/block_break_event_component.hpp"
#include "components/block_place_event_component.hpp"
#include "components/block_event_component.hpp"

class Application {
    public:
        Application() :
            mThreadPool(ThreadPool::GetInstance()),
            mEventManager(EventManager::GetInstance()),
            mGraphicsApp(std::make_shared<GraphicsApp>())
        {}

        void run();

    private:
        void input(float deltaTime);
        void mainLoop(float deltaTime);
        void system(float deltaTime);

        void fpsCounter(float deltaTime);

        void initialize();
        void initializeWorld();
        void initializeKeys();
        void initializeBaseEntities();
        void initializeTextures();
        void initializeBlocks();

        glm::vec3 getBlock(
            bismuth::Registry      & registry, 
            PositionComponent const& playerPos, 
            CameraComponent   const& playerCam, 
            float                    epsilon
        );

    private:
        Settings mSettings;
        Game mGame;

        std::shared_ptr<GraphicsApp> mGraphicsApp;
        bismuth::Registry mRegistry;
        PlayerControllerSystem mPlayerControllerSys;

        // ChunkManager& gChunkManager = ChunkManager::GetInstance();

        // ChunkSystem chunkSystem;
        ThreadPool& mThreadPool;

        EventManager& mEventManager;
        tbb::task_arena mArena;

        unsigned int mSeed = 123456789;
};