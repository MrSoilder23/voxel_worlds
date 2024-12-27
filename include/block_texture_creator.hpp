#pragma once
// C++ standard libraries
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <utility>

// Third_party libraries
#include <glad/glad.h>

// Own libraries
#include <texture.hpp>

class BlockTextureCreator {
    public:
        ~BlockTextureCreator();

        void CreateTexture(std::string name, SDL_Surface* surface);
        unsigned int& GetTexture(std::string name);

        void CreateBlockFromAtlas(std::string name, std::string atlas, std::vector<GLfloat> corners);
        std::vector<GLfloat>& GetBlockFromAtlas(std::string name);

        static BlockTextureCreator& GetInstance();
        
    private:
        BlockTextureCreator();
        BlockTextureCreator(BlockTextureCreator const&);
        BlockTextureCreator operator=(BlockTextureCreator const& rhs);
    
    private:
        std::unordered_map<std::string, std::pair<GLuint, SDL_Surface*>> mTextures;
        std::unordered_map<std::string, std::vector<GLfloat>> mBlockTexture;
};