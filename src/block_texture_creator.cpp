#include "block_texture_creator.hpp"

BlockTextureCreator::BlockTextureCreator() {}
BlockTextureCreator::BlockTextureCreator(BlockTextureCreator const&) {}
BlockTextureCreator BlockTextureCreator::operator=(BlockTextureCreator const& rhs) {
    return *this;
}

BlockTextureCreator::~BlockTextureCreator() {
    std::cout << "BlockTextureCreator bye bye" << std::endl;
}

void BlockTextureCreator::CreateTexture(std::string name, SDL_Surface* surface) {
    mTextures[name].second = surface;
    
    glGenTextures(1, &mTextures[name].first);
    glBindTexture(GL_TEXTURE_2D, mTextures[name].first);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
unsigned int& BlockTextureCreator::GetTexture(std::string name) {
    return mTextures[name].first;
}

void BlockTextureCreator::CreateBlockFromAtlas(std::string name, std::string atlas, std::vector<GLfloat> corners) {
    mBlockTexture[name] = { 
            // FrontFace
            corners[1]/mTextures[atlas].second->w, corners[3]/mTextures[atlas].second->h,
            corners[0]/mTextures[atlas].second->w, corners[3]/mTextures[atlas].second->h,
            corners[1]/mTextures[atlas].second->w, corners[2]/mTextures[atlas].second->h,
            corners[0]/mTextures[atlas].second->w, corners[2]/mTextures[atlas].second->h,

            // BackFace
            corners[5]/mTextures[atlas].second->w, corners[7]/mTextures[atlas].second->h,
            corners[4]/mTextures[atlas].second->w, corners[7]/mTextures[atlas].second->h,
            corners[5]/mTextures[atlas].second->w, corners[6]/mTextures[atlas].second->h,
            corners[4]/mTextures[atlas].second->w, corners[6]/mTextures[atlas].second->h,
            
            // LeftFace
            corners[8]/mTextures[atlas].second->w, corners[10]/mTextures[atlas].second->h,
            corners[8]/mTextures[atlas].second->w, corners[11]/mTextures[atlas].second->h,
            corners[9]/mTextures[atlas].second->w, corners[11]/mTextures[atlas].second->h,
            corners[9]/mTextures[atlas].second->w, corners[10]/mTextures[atlas].second->h,

            // RightFace
            corners[12]/mTextures[atlas].second->w, corners[14]/mTextures[atlas].second->h,
            corners[12]/mTextures[atlas].second->w, corners[15]/mTextures[atlas].second->h,
            corners[13]/mTextures[atlas].second->w, corners[15]/mTextures[atlas].second->h,
            corners[13]/mTextures[atlas].second->w, corners[14]/mTextures[atlas].second->h,

            // TopFace
            corners[16]/mTextures[atlas].second->w, corners[18]/mTextures[atlas].second->h,
            corners[16]/mTextures[atlas].second->w, corners[19]/mTextures[atlas].second->h,
            corners[17]/mTextures[atlas].second->w, corners[19]/mTextures[atlas].second->h,
            corners[17]/mTextures[atlas].second->w, corners[18]/mTextures[atlas].second->h,

            // BotFace
            corners[20]/mTextures[atlas].second->w, corners[22]/mTextures[atlas].second->h,
            corners[21]/mTextures[atlas].second->w, corners[22]/mTextures[atlas].second->h,
            corners[20]/mTextures[atlas].second->w, corners[23]/mTextures[atlas].second->h,
            corners[21]/mTextures[atlas].second->w, corners[23]/mTextures[atlas].second->h,
    };
}
std::vector<GLfloat>& BlockTextureCreator::GetBlockFromAtlas(std::string name) {
    return mBlockTexture[name];
}

BlockTextureCreator& BlockTextureCreator::GetInstance() {
    static BlockTextureCreator sInstance;
    return sInstance;
}