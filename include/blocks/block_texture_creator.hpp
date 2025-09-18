#pragma once
// C++ standard libraries
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <memory>

// Third_party libraries
#include <glad/glad.h>
#include <SDL2/SDL_image.h>

// Own libraries
#include "components/core/material.hpp"

class BlockTextureCreator {
    public:
        ~BlockTextureCreator();

        void createTexture(std::string name, SDL_Surface* faces);

        std::shared_ptr<Texture> getTexture(std::string name);
        std::unordered_map<std::string, std::shared_ptr<Texture>>& getTextures();

        static BlockTextureCreator& getInstance();
        
    private:
        BlockTextureCreator();
        BlockTextureCreator(BlockTextureCreator const&);
        BlockTextureCreator operator=(BlockTextureCreator const& rhs);
    
    private:
        std::unordered_map<std::string, std::shared_ptr<Texture>> mTextureIDs;
};