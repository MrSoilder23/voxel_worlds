#include "blocks.hpp"
#include <memory>
#include "graphics.hpp"

void InitializeModels() {
    ModelManager& modelManager = ModelManager::GetInstance();

    Model block;

    block.vertexPositions = {
        // FrontFace
        glm::vec3(-0.5f, -0.5f,  0.5f), // BotLeftVertex
        glm::vec3( 0.5f, -0.5f,  0.5f), // BotRightVertex
        glm::vec3(-0.5f,  0.5f,  0.5f), // TopLeftVertex
        glm::vec3( 0.5f,  0.5f,  0.5f), // TopRightVertex

        // BackFace
        glm::vec3(-0.5f, -0.5f, -0.5f), // BackBotLeft
        glm::vec3( 0.5f, -0.5f, -0.5f), // BackBotRight
        glm::vec3(-0.5f,  0.5f, -0.5f), // BackTopLeft
        glm::vec3( 0.5f,  0.5f, -0.5f), // BackTopRight
    };

    // block.indexBufferData = {
    //     0,1,2, 3,2,1, // FrontFace
    //     0,4,5, 1,0,5, // BottomFace
    //     6,7,4, 7,5,4, // BackFace
    //     6,2,7, 3,7,2, // TopFace
    //     3,1,7, 5,7,1, // RightFace
    //     6,4,2, 0,2,4, // LeftFace
    // };

    block.indexBufferData = {};

    modelManager.CreateNewModel("cube", block);
}

void InitializeTextures() {
    TextureManager& textureManager = TextureManager::GetInstance();
    BlockTextureCreator& blockTextureCreator = BlockTextureCreator::GetInstance();

    SDL_Surface* grassTop = IMG_Load("./assets/grass_top.png");
    SDL_Surface* grassSide = IMG_Load("./assets/grass_side.png");
    SDL_Surface* grassBottom = IMG_Load("./assets/grass_bottom.png");

    textureManager.CreateNewTexture("grassTop", grassTop);
    textureManager.CreateNewTexture("grassSide", grassSide);
    textureManager.CreateNewTexture("grassBottom", grassBottom);

    std::vector<SDL_Surface*> grassBlockFaces;
    grassBlockFaces.reserve(6); // Right Left Top Bottom Back Front
    grassBlockFaces.push_back(textureManager.GetTexture("grassSide"));
    grassBlockFaces.push_back(textureManager.GetTexture("grassSide"));
    grassBlockFaces.push_back(textureManager.GetTexture("grassTop"));
    grassBlockFaces.push_back(textureManager.GetTexture("grassBottom"));
    grassBlockFaces.push_back(textureManager.GetTexture("grassSide"));
    grassBlockFaces.push_back(textureManager.GetTexture("grassSide"));

    std::vector<SDL_Surface*> dirtBlockFaces;
    dirtBlockFaces.reserve(6); // Right Left Top Bottom Back Front
    dirtBlockFaces.push_back(textureManager.GetTexture("grassBottom"));
    dirtBlockFaces.push_back(textureManager.GetTexture("grassBottom"));
    dirtBlockFaces.push_back(textureManager.GetTexture("grassBottom"));
    dirtBlockFaces.push_back(textureManager.GetTexture("grassBottom"));
    dirtBlockFaces.push_back(textureManager.GetTexture("grassBottom"));
    dirtBlockFaces.push_back(textureManager.GetTexture("grassBottom"));

    blockTextureCreator.CreateTexture("grass_block", grassBlockFaces);
    blockTextureCreator.CreateTexture("dirt_block", dirtBlockFaces);
}

void InitializeBlocks() {
    EntityManager& entityManager = EntityManager::GetInstance();
    ModelManager& modelManager = ModelManager::GetInstance();
    BlockTextureCreator& blockTextureCreator = BlockTextureCreator::GetInstance();

    GLfloat defaultPos[] = {0.0f,0.0f,0.0f};

    // Grass block
    std::string grassBlock = "grass_block";
    entityManager.CreateEntity(grassBlock);
    entityManager.AddComponent<ModelComponent>(grassBlock);
    entityManager.AddComponent<PositionComponent>(grassBlock);

    ModelComponent& grassModel = *entityManager.GetComponent<ModelComponent>(grassBlock);
    grassModel.mModel = modelManager.GetModel("cube");

    std::shared_ptr<Texture> grassBlockTexture = std::make_shared<Texture>();
    grassBlockTexture->textureHandle = blockTextureCreator.GetTexture("grass_block");
    grassModel.mTextures = grassBlockTexture;


    // Dirt Block
    std::string dirtBlock = "dirt_block";
    entityManager.CreateEntity(dirtBlock);
    entityManager.AddComponent<ModelComponent>(dirtBlock);
    entityManager.AddComponent<PositionComponent>(dirtBlock);

    ModelComponent& dirtModel = *entityManager.GetComponent<ModelComponent>(dirtBlock);
    dirtModel.mModel = modelManager.GetModel("cube");

    std::shared_ptr<Texture> dirtBlockTexture = std::make_shared<Texture>();
    dirtBlockTexture->textureHandle = blockTextureCreator.GetTexture("dirt_block");
    dirtModel.mTextures = dirtBlockTexture;
    
    entityManager.InitializeAllComponents();
}
