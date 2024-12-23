#include "blocks.hpp"

void InitializeModels() {
    ModelManager& modelManager = ModelManager::GetInstance();

    std::shared_ptr<Model> block = std::make_shared<Model>();

    block->vertexPositions = {
        -0.5f, -0.5f,  0.5f, // BotLeftVertex
         0.5f, -0.5f,  0.5f, // BotRightVertex
        -0.5f,  0.5f,  0.5f, // TopLeftVertex
         0.5f,  0.5f,  0.5f, // TopRightVertex
    
        -0.5f, -0.5f, -0.5f, // BackBotLeft
         0.5f, -0.5f, -0.5f, // BackBotRight
        -0.5f,  0.5f, -0.5f, // BackTopLeft
         0.5f,  0.5f, -0.5f, // BackTopRight
    };
    block->indexBufferData = {
        0,1,2, 3,2,1, // FrontFace
        0,4,5, 1,0,5, // BottomFace
        6,7,4, 7,5,4, // BackFace
        6,2,7, 3,7,2, // TopFace
        3,1,7, 5,7,1, // RightFace
        6,4,2, 0,2,4, // LeftFace
    };

    modelManager.CreateNewModel("cube", block);
    
}

void InitializeTextures() {
    TextureManager& textureManager = TextureManager::GetInstance();
    BlockTextureCreator& blockTextureCreator = BlockTextureCreator::GetInstance();

    // std::shared_ptr<Texture> testTexture = std::make_shared<Texture>();
    SDL_Surface* grassTop = IMG_Load("./assets/grass_top.png");
    SDL_Surface* grassSide = IMG_Load("./assets/grass_side.png");
    SDL_Surface* grassBottom = IMG_Load("./assets/grass_bottom.png");

    textureManager.CreateNewTexture("grassTop", grassTop);
    textureManager.CreateNewTexture("grassSide", grassSide);
    textureManager.CreateNewTexture("grassBottom", grassBottom);

    std::vector<SDL_Surface*> faces;
    faces.reserve(6); // Right Left Top Bottom Back Front
    faces.push_back(textureManager.GetTexture("grassSide"));
    faces.push_back(textureManager.GetTexture("grassSide"));
    faces.push_back(textureManager.GetTexture("grassTop"));
    faces.push_back(textureManager.GetTexture("grassBottom"));
    faces.push_back(textureManager.GetTexture("grassSide"));
    faces.push_back(textureManager.GetTexture("grassSide"));

    blockTextureCreator.CreateTexture("test_block", faces);

}

void InitializeBlocks() {
    EntityManager& entityManager = EntityManager::GetInstance();
    ModelManager& modelManager = ModelManager::GetInstance();
    BlockTextureCreator& blockTextureCreator = BlockTextureCreator::GetInstance();

    GLfloat defaultPos[] = {0.0f,0.0f,0.0f};

    std::string grassBlock = "grass_block";
    entityManager.CreateEntity(grassBlock);

    entityManager.AddComponent<ModelComponent>(grassBlock);
    entityManager.AddComponent<PositionComponent>(grassBlock);

    ModelComponent& grassModel = *entityManager.GetComponent<ModelComponent>(grassBlock);

    grassModel.AddModel(modelManager.GetModel("cube"));

    std::shared_ptr<Texture> grassBlockTexture = std::make_shared<Texture>();
    grassBlockTexture->textureID = blockTextureCreator.GetTexture("test_block");
    grassModel.AddTextures(grassBlockTexture);
    
    grassModel.GetMeshData().Initialize(grassModel.GetModel(), grassModel.GetTexture());
    
    
    entityManager.InitializeAllComponents();
}
