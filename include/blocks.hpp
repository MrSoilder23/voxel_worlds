#pragma once
// C++ standard libraries
#include <string>

// Third_party libraries
#include <SDL2/SDL_image.h>

// Own libraries
#include "entity_manager.hpp"
#include "model_manager.hpp"
#include "texture_manager.hpp"
#include "block_texture_creator.hpp"
// Components
#include "model_component.hpp"
#include "position_component.hpp"

void InitializeBlocks();
void InitializeTextures();
void InitializeModels();