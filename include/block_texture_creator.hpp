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
#include <texture.hpp>

class BlockTextureCreator {
    public:
        ~BlockTextureCreator();

        void CreateTexture(std::string name, std::vector<SDL_Surface*> faces);

        GLuint64& GetTexture(std::string name);

        static BlockTextureCreator& GetInstance();
        
    private:
        BlockTextureCreator();
        BlockTextureCreator(BlockTextureCreator const&);
        BlockTextureCreator operator=(BlockTextureCreator const& rhs);
    
    private:
        std::unordered_map<std::string, GLuint64> mCubeTexture;
};