#include "./systems/texture_manager.hpp"

TextureManager::TextureManager() {}
TextureManager::TextureManager(TextureManager const&) {}
TextureManager TextureManager::operator=(TextureManager const& rhs) {
    return *this;
}

TextureManager& TextureManager::GetInstance() {
    static TextureManager sInstance;
    return sInstance;
}
TextureManager::~TextureManager() {
    std::cout << "TextureManager bye bye" << std::endl;
    for(auto [name, image] : mTextures) {
        SDL_FreeSurface(image);
    }
}

void TextureManager::CreateNewTexture(const std::string& name, SDL_Surface* texture) {
    mTextures[name] = texture;
}

SDL_Surface* TextureManager::GetTexture(const std::string& name) {
    auto search = mTextures.find(name);
    if(search != mTextures.end()) {
        return search->second;
    } else {
        std::cerr << "Could not find the texture: " << name << std::endl;
        return nullptr;
    }
}
