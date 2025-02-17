#pragma once
// C++ standard libraries
#include <string>

// Third_party libraries
#include <SDL2/SDL_image.h>

// Own libraries
#include "./core/entity_manager.hpp"
#include "./graphics/model_manager.hpp"
#include "./systems/texture_manager.hpp"
#include "block_texture_creator.hpp"
#include "block_types.hpp"
#include "block_registry.hpp"
// Components
#include "./components/model_component.hpp"
#include "./components/position_component.hpp"

void InitializeBlocks();
void InitializeTextures();
void InitializeModels();