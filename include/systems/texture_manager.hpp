#pragma once
// C++ standard libraries
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

// Third_party libraries
#include <SDL2/SDL_image.h>

class TextureManager {
    public:
        static TextureManager& GetInstance();
        ~TextureManager();
        
        void CreateNewTexture(const std::string& name, SDL_Surface* texture);

        SDL_Surface* GetTexture(const std::string& name);

    private:
        TextureManager();
        TextureManager(TextureManager const&);
        TextureManager operator=(TextureManager const& rhs);

    private:
        std::unordered_map<std::string, SDL_Surface*> mTextures;
};