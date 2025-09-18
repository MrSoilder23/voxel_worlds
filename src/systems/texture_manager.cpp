#include "./systems/texture_manager.hpp"

TextureManager::TextureManager() {}
TextureManager::TextureManager(TextureManager const&) {}
TextureManager TextureManager::operator=(TextureManager const& rhs) {
    return *this;
}

TextureManager& TextureManager::getInstance() {
    static TextureManager sInstance;
    return sInstance;
}
TextureManager::~TextureManager() {
    std::cout << "TextureManager bye bye" << std::endl;
    mTextures.clear();
}

void TextureManager::createNewTexture(const std::string& name, SDL_Surface* surface) {
    assert(surface != nullptr && "Invalid surface provided for texture creation");

    int width, height, channels;
    GLenum format;
    GLuint textureID = createGLTextureFromSurface(surface, width, height, channels, format);

    assert(textureID != 0 && "Failed to create OpenGL texture from surface");

    auto texture = std::make_shared<Texture>();
    texture->id = textureID;
    texture->width = width;
    texture->height = height;
    texture->channels = channels;
    texture->format = format;

    mTextures[name] = texture;

}

std::shared_ptr<Texture> TextureManager::getTexture(const std::string& name) {
    auto search = mTextures.find(name);
    if(search != mTextures.end()) {
        return search->second;
    } else {
        std::cerr << "Could not find the texture: " << name << std::endl;
        return nullptr;
    }
}

// Private
GLuint TextureManager::createGLTextureFromSurface(SDL_Surface* surface, int& width, int& height, int& channels, GLenum& format) {
    if (!surface) return 0;
    
    width = surface->w;
    height = surface->h;
    channels = surface->format->BytesPerPixel;
    
    if (channels == 4) {
        format = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
    } else if (channels == 3) {
        format = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
    } else {
        std::cerr << "Texture has unsupported number of channels: " << channels << std::endl;
        return 0;
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return textureID;
}