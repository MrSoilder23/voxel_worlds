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

void InitializeBlocks() {
    EntityManager& entityManager = EntityManager::GetInstance();
    ModelManager& modelManager = ModelManager::GetInstance();

    GLfloat defaultPos[] = {0.0f,0.0f,0.0f};

    std::string grassBlock = "grass_block";
    entityManager.CreateEntity(grassBlock);

    entityManager.AddComponent<ModelComponent>(grassBlock);
    entityManager.AddComponent<PositionComponent>(grassBlock);

    ModelComponent& grassModel = *entityManager.GetComponent<ModelComponent>(grassBlock);

    grassModel.AddModel(modelManager.GetModel("cube"));
    
    grassModel.GetMeshData().Initialize(grassModel.GetModel(), grassModel.GetTexture());
    
    
    entityManager.InitializeAllComponents();
}
