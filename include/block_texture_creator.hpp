#pragma once
// C++ standard libraries
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

// Third_party libraries
#include <glad/glad.h>

// Own libraries
#include <texture.hpp>

class BlockTextureCreator {
    public:
        ~BlockTextureCreator();

        void CreateTexture(std::string name, std::vector<SDL_Surface*> faces);
        unsigned int& GetTexture(std::string name);

        static BlockTextureCreator& GetInstance();
        
    private:
        BlockTextureCreator();
        BlockTextureCreator(BlockTextureCreator const&);
        BlockTextureCreator operator=(BlockTextureCreator const& rhs);
    
    private:
        std::unordered_map<std::string, unsigned int> mCubeTexture;
};