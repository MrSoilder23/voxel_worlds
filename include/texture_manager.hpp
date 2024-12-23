#pragma once
// C++ standard libraries
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

// Own libraries
#include "texture.hpp"

class TextureManager {
    public:
        static TextureManager& GetInstance();
        ~TextureManager();
        
        void CreateNewTexture(const std::string& name, const std::shared_ptr<Texture>& texture);

        std::shared_ptr<Texture> GetTexture(const std::string& name);

    private:
        TextureManager();
        TextureManager(TextureManager const&);
        TextureManager operator=(TextureManager const& rhs);

    private:
        std::unordered_map<std::string, std::shared_ptr<Texture>> mTextures;
};