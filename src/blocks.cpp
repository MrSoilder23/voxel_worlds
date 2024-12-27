#include "blocks.hpp"
#include <memory>
#include "graphics.hpp"

void InitializeModels() {
    ModelManager& modelManager = ModelManager::GetInstance();

    std::shared_ptr<Model> block = std::make_shared<Model>();

    block->vertexPositions = {

        // FrontFace
        -0.5f, -0.5f,  0.5f, // BotLeftVertex       0
         0.5f, -0.5f,  0.5f, // BotRightVertex      1
        -0.5f,  0.5f,  0.5f, // TopLeftVertex       2
         0.5f,  0.5f,  0.5f, // TopRightVertex      3

        // BackFace
        -0.5f, -0.5f, -0.5f, // BackBotLeft         4
         0.5f, -0.5f, -0.5f, // BackBotRight        5
        -0.5f,  0.5f, -0.5f, // BackTopLeft         6
         0.5f,  0.5f, -0.5f, // BackTopRight        7

        // LeftFace
        -0.5f,  0.5f, -0.5f, //                     8
        -0.5f, -0.5f, -0.5f, //                     9
        -0.5f, -0.5f,  0.5f, //                     10
        -0.5f,  0.5f,  0.5f, //                     11

        // RightFace
         0.5f,  0.5f,  0.5f, //                     12
         0.5f, -0.5f,  0.5f, //                     13
         0.5f, -0.5f, -0.5f, //                     14
         0.5f,  0.5f, -0.5f, //                     15

        // TopFace
        -0.5f,  0.5f, -0.5f, //                     16
        -0.5f,  0.5f,  0.5f, //                     17
         0.5f,  0.5f,  0.5f, //                     18
         0.5f,  0.5f, -0.5f, //                     19

        // BotFace
        -0.5f, -0.5f, -0.5f, //                     20
        -0.5f, -0.5f,  0.5f, //                     21
         0.5f, -0.5f,  0.5f, //                     22
         0.5f, -0.5f, -0.5f, //                     23


    };
    block->indexBufferData = {
        0,1,2, 3,2,1, // FrontFace
        6,7,4, 7,5,4, // BackFace
        8,9,10, 11,8,10, // LeftFace
        12,13,14, 15,12,14, // RightFace
        16,17,18, 19,16,18, // TopFace
        22,21,20, 22,20,23 // BotFace

    };

    modelManager.CreateNewModel("cube", block);
    
}

void InitializeTextures() {
    TextureManager& textureManager = TextureManager::GetInstance();
    BlockTextureCreator& blockTextureCreator = BlockTextureCreator::GetInstance();

    SDL_Surface* textureAtlas = IMG_Load("./assets/atlas.png");

    textureManager.CreateNewTexture("atlas", textureAtlas);
    blockTextureCreator.CreateTexture("atlas", textureAtlas);

    // Front,Back,Left,Right,Top,Bot
    std::vector<GLfloat> grassCoordinates = {
        0.0f,32.0f, 32.0f,64.0f, // Side
        0.0f,32.0f, 32.0f,64.0f, // Side
        0.0f,32.0f, 32.0f,64.0f, // Side
        0.0f,32.0f, 32.0f,64.0f, // Side
        0.0f,32.0f, 0.0f,32.0f,  // Top
        0.0f,32.0f, 64.0f,96.0f, // Bot
    };
    std::vector<GLfloat> dirtCoordinates = {
        0.0f,32.0f, 64.0f,96.0f,
        0.0f,32.0f, 64.0f,96.0f,
        0.0f,32.0f, 64.0f,96.0f,
        0.0f,32.0f, 64.0f,96.0f,
        0.0f,32.0f, 64.0f,96.0f,
        0.0f,32.0f, 64.0f,96.0f,
    };

    blockTextureCreator.CreateBlockFromAtlas("grass_block", "atlas", grassCoordinates);
    blockTextureCreator.CreateBlockFromAtlas("dirt_block", "atlas", dirtCoordinates);
}

void InitializeBlocks() {
    EntityManager& entityManager = EntityManager::GetInstance();
    ModelManager& modelManager = ModelManager::GetInstance();
    BlockTextureCreator& blockTextureCreator = BlockTextureCreator::GetInstance();
    TextureManager& textureManager = TextureManager::GetInstance();

    GLfloat defaultPos[] = {0.0f,0.0f,0.0f};

    // Grass block
    std::string grassBlock = "grass_block";
    entityManager.CreateEntity(grassBlock);
    entityManager.AddComponent<ModelComponent>(grassBlock);
    entityManager.AddComponent<PositionComponent>(grassBlock);

    ModelComponent& grassModel = *entityManager.GetComponent<ModelComponent>(grassBlock);
    grassModel.AddModel(modelManager.GetModel("cube"));

    std::shared_ptr<Texture> grassBlockTexture = std::make_shared<Texture>();
    grassBlockTexture->texturePositions = blockTextureCreator.GetBlockFromAtlas("grass_block");
    grassModel.AddTextures(grassBlockTexture);


    // Dirt Block
    std::string dirtBlock = "dirt_block";
    entityManager.CreateEntity(dirtBlock);
    entityManager.AddComponent<ModelComponent>(dirtBlock);
    entityManager.AddComponent<PositionComponent>(dirtBlock);

    ModelComponent& dirtModel = *entityManager.GetComponent<ModelComponent>(dirtBlock);
    dirtModel.AddModel(modelManager.GetModel("cube"));

    std::shared_ptr<Texture> dirtBlockTexture = std::make_shared<Texture>();
    dirtBlockTexture->texturePositions = blockTextureCreator.GetBlockFromAtlas("dirt_block");
    dirtModel.AddTextures(dirtBlockTexture);


    grassModel.GetMeshData().Initialize(grassModel.GetModel(), grassModel.GetTexture());
    dirtModel.GetMeshData().Initialize(dirtModel.GetModel(), dirtModel.GetTexture());
    
    entityManager.InitializeAllComponents();
}
