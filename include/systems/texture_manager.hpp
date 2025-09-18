#pragma once
// C++ standard libraries
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include <cassert>

// Third_party libraries
#include <SDL2/SDL_image.h>

// Own libraries
#include "components/core/material.hpp"

class TextureManager {
    public:
        static TextureManager& getInstance();
        ~TextureManager();
        
        void createNewTexture(const std::string& name, SDL_Surface* surface);

        std::shared_ptr<Texture> getTexture(const std::string& name);

    private:
        TextureManager();
        TextureManager(TextureManager const&);
        TextureManager operator=(TextureManager const& rhs);

        GLuint createGLTextureFromSurface(SDL_Surface* surface, int& width, int& height, int& channels, GLenum& format);

    private:
        std::unordered_map<std::string, std::shared_ptr<Texture>> mTextures;
};