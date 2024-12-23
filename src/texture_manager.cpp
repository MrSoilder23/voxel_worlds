#include "texture_manager.hpp"

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
}

void TextureManager::CreateNewTexture(const std::string& name, const std::shared_ptr<Texture>& texture) {
    mTextures[name] = texture;
}

std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& name) {
    auto search = mTextures.find(name);
    if(search != mTextures.end()) {
        return search->second;
    } else {
        std::cerr << "Could not find the texture: " << name << std::endl;
        return nullptr;
    }
}
