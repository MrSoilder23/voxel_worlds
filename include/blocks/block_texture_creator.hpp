#pragma once
// C++ standard libraries
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

// Third_party libraries
#include <glad/glad.h>
#include <SDL2/SDL_image.h>

// Own libraries
#include "./graphics/texture.hpp"

class BlockTextureCreator {
    public:
        ~BlockTextureCreator();

        void CreateTexture(std::string name, SDL_Surface* faces);

        GLuint& GetTexture(std::string name);
        std::unordered_map<std::string, GLuint>& GetTextures();

        static BlockTextureCreator& GetInstance();
        
    private:
        BlockTextureCreator();
        BlockTextureCreator(BlockTextureCreator const&);
        BlockTextureCreator operator=(BlockTextureCreator const& rhs);
    
    private:
        std::unordered_map<std::string, GLuint> mTextureIDs;
};