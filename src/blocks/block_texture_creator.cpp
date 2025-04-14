#include "./blocks/block_texture_creator.hpp"

BlockTextureCreator::BlockTextureCreator() {}
BlockTextureCreator::BlockTextureCreator(BlockTextureCreator const&) {}
BlockTextureCreator BlockTextureCreator::operator=(BlockTextureCreator const& rhs) {
    return *this;
}

BlockTextureCreator::~BlockTextureCreator() {
    std::cout << "BlockTextureCreator bye bye" << std::endl;
}

void BlockTextureCreator::CreateTexture(std::string name, SDL_Surface* image) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    mTextureIDs[name] = texture;
}

GLuint& BlockTextureCreator::GetTexture(std::string name) {
    return mTextureIDs[name];
}
std::unordered_map<std::string, GLuint>& BlockTextureCreator::GetTextures() {
    return mTextureIDs;
}

BlockTextureCreator& BlockTextureCreator::GetInstance() {
    static BlockTextureCreator sInstance;
    return sInstance;
}