#include "./blocks/block_texture_creator.hpp"

BlockTextureCreator::BlockTextureCreator() {}
BlockTextureCreator::BlockTextureCreator(BlockTextureCreator const&) {}
BlockTextureCreator BlockTextureCreator::operator=(BlockTextureCreator const& rhs) {
    return *this;
}

BlockTextureCreator::~BlockTextureCreator() {
    std::cout << "BlockTextureCreator bye bye" << std::endl;
}

void BlockTextureCreator::CreateTexture(std::string name, std::vector<SDL_Surface*> faces) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    for(unsigned int i = 0; i < faces.size(); i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, faces[i]->w, faces[i]->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, faces[i]->pixels);
    }

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 7);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, 0.7);

    mCubeTexture[name] = glGetTextureHandleARB(texture);
    glMakeTextureHandleResidentARB(mCubeTexture[name]);
    std::cout << name << ": " << mCubeTexture[name] << std::endl;
}

GLuint64& BlockTextureCreator::GetTexture(std::string name) {
    return mCubeTexture[name];
}
std::unordered_map<std::string, GLuint64>& BlockTextureCreator::GetTextures() {
    return mCubeTexture;
}

BlockTextureCreator& BlockTextureCreator::GetInstance() {
    static BlockTextureCreator sInstance;
    return sInstance;
}