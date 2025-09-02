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
#include <tbb/tbb.h>

// Own libraries
#include "core/entity_manager.hpp"
#include "core/game.hpp"
#include "core/event_manager.hpp"
#include "utility/settings.hpp"
#include "utility/thread_pool.hpp"
#include "utility/circle_loop.hpp"
#include "graphics/graphics.hpp"
#include "graphics/model_manager.hpp"

#include "blocks/block_texture_creator.hpp"
#include "blocks/block_types.hpp"
#include "blocks/block_registry.hpp"

#include "systems/renderer_system.hpp"
#include "systems/player_controller_system.hpp"
#include "systems/vertex_setup_system.hpp"
#include "systems/bounding_box_system.hpp"
#include "systems/position_update_system.hpp"
#include "systems/chunk_vertex_setup_system.hpp"
#include "systems/world_generation_system.hpp"
#include "systems/chunk_bounding_creation_system.hpp"
#include "systems/collision_system.hpp"
#include "systems/physics_system.hpp"
#include "systems/player_target_system.hpp"
#include "systems/chunk_meshing_system.hpp"
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
#include "components/model_component.hpp"
#include "components/position_component.hpp"

class Application {
    public:
        Application() : 
            mEntityManager(EntityManager::GetInstance()),
            mRendererSystem(RendererSystem::GetInstance()),
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
        void initializeRender();
        void initializeModels();
        void initializeTextures();
        void initializeBlocks();

    private:
        Settings mSettings;
        Game mGame;

        std::shared_ptr<GraphicsApp> mGraphicsApp;
        EntityManager& mEntityManager;
        PlayerControllerSystem mPlayerControllerSys;
        ChunkBoundingCreationSystem mChunkbBoxCreationSys;

        RendererSystem& mRendererSystem;
        // ChunkManager& gChunkManager = ChunkManager::GetInstance();

        // ChunkSystem chunkSystem;
        ThreadPool& mThreadPool;
        WorldGenerationSystem mWorldGen;

        EventManager& mEventManager;
        tbb::task_arena mArena;

        unsigned int mSeed;
};