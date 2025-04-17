#include "./blocks/blocks.hpp"

void InitializeModels() {
    ModelManager& modelManager = ModelManager::GetInstance();

    Model block;

    block.vertexPositions = {
        // Front Face
        glm::vec3(-0.5f, -0.5f,  0.5f), // 0  - Bottom-left
        glm::vec3( 0.5f, -0.5f,  0.5f), // 1  - Bottom-right
        glm::vec3(-0.5f,  0.5f,  0.5f), // 2  - Top-left
        glm::vec3( 0.5f,  0.5f,  0.5f), // 3  - Top-right

        // Back Face
        glm::vec3(-0.5f, -0.5f, -0.5f), // 4  - Bottom-left
        glm::vec3( 0.5f, -0.5f, -0.5f), // 5  - Bottom-right
        glm::vec3(-0.5f,  0.5f, -0.5f), // 6  - Top-left
        glm::vec3( 0.5f,  0.5f, -0.5f), // 7  - Top-right

        // Right Face
        glm::vec3( 0.5f, -0.5f,  0.5f), //8
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f), //11

        // Left Face
        glm::vec3(-0.5f, -0.5f, -0.5f), //12
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f), //15

        // Top Face
        glm::vec3(-0.5f,  0.5f,  0.5f), //16
        glm::vec3( 0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3( 0.5f,  0.5f, -0.5f), //19

        // Bottom Face
        glm::vec3(-0.5f, -0.5f, -0.5f), //20
        glm::vec3( 0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3( 0.5f, -0.5f,  0.5f), //23
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

    SDL_Surface* chunkAtlas = IMG_Load("./assets/AtlasMap.png");


    if (!chunkAtlas) {
        std::cerr << "Failed to load textures!" << std::endl;
        return;
    }

    textureManager.CreateNewTexture("ChunkTexture", chunkAtlas);

    blockTextureCreator.CreateTexture("ChunkTexture", chunkAtlas);
}

void InitializeBlocks() {
    BlockRegistry& blockRegistry = BlockRegistry::GetInstance();
    ModelManager& modelManager = ModelManager::GetInstance();

    // Grass block
    BlockTemplate grassBlock;

    grassBlock.textureCoords[0] = glm::vec2(1,0);
    grassBlock.textureCoords[1] = glm::vec2(1,0);
    grassBlock.textureCoords[2] = glm::vec2(1,0);
    grassBlock.textureCoords[3] = glm::vec2(1,0);
    grassBlock.textureCoords[4] = glm::vec2(2,0);
    grassBlock.textureCoords[5] = glm::vec2(0,0);

    grassBlock.model = modelManager.GetModel("cube");
    blockRegistry.RegisterBlock(BlockTypes::grass_block, grassBlock);

    // Dirt Block    
    BlockTemplate dirtBlock;

    dirtBlock.textureCoords[0] = glm::vec2(0,0);
    dirtBlock.textureCoords[1] = glm::vec2(0,0);
    dirtBlock.textureCoords[2] = glm::vec2(0,0);
    dirtBlock.textureCoords[3] = glm::vec2(0,0);
    dirtBlock.textureCoords[4] = glm::vec2(0,0);
    dirtBlock.textureCoords[5] = glm::vec2(0,0);

    dirtBlock.model = modelManager.GetModel("cube");
    blockRegistry.RegisterBlock(BlockTypes::dirt_block, dirtBlock);

    // Stone Block    
    BlockTemplate stoneBlock;

    stoneBlock.textureCoords[0] = glm::vec2(4,0);
    stoneBlock.textureCoords[1] = glm::vec2(4,0);
    stoneBlock.textureCoords[2] = glm::vec2(4,0);
    stoneBlock.textureCoords[3] = glm::vec2(4,0);
    stoneBlock.textureCoords[4] = glm::vec2(4,0);
    stoneBlock.textureCoords[5] = glm::vec2(4,0);

    stoneBlock.model = modelManager.GetModel("cube");
    blockRegistry.RegisterBlock(BlockTypes::stone_block, stoneBlock);

    // Sand Block    
    BlockTemplate sandBlock;

    sandBlock.textureCoords[0] = glm::vec2(3,0);
    sandBlock.textureCoords[1] = glm::vec2(3,0);
    sandBlock.textureCoords[2] = glm::vec2(3,0);
    sandBlock.textureCoords[3] = glm::vec2(3,0);
    sandBlock.textureCoords[4] = glm::vec2(3,0);
    sandBlock.textureCoords[5] = glm::vec2(3,0);

    sandBlock.model = modelManager.GetModel("cube");
    blockRegistry.RegisterBlock(BlockTypes::sand_block, sandBlock);
    
}
