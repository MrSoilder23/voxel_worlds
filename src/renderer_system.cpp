#include "renderer_system.hpp"
#include <iostream>
#include "utility.hpp"

RendererSystem::~RendererSystem() {
    std::cout << "RendererSystem bye bye" << std::endl;
}

void RendererSystem::DrawAll() {
    EntityManager& entityManager = EntityManager::GetInstance();
    for(auto it = entityManager.GetEntities().begin(); it != entityManager.GetEntities().end(); ++it) {
        auto componentPointer = entityManager.GetComponent<ModelComponent>(it->first);

        if(componentPointer) { 
            glUseProgram(mGraphicsApp->mGraphicsPipeline);

            GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
            glUniformMatrix4fv(uModelMatrixLocation, 1, false, &componentPointer->GetTransform().mModelMatrix[0][0]);

            glm::mat4 view = mGraphicsApp->mCamera.GetViewMatrix();
            GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
            glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);

            glm::mat4 perspective = mGraphicsApp->mCamera.GetProjectionMatrix();
            GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
            glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);

            componentPointer->GetMeshData().Bind();

            glDrawElements(GL_TRIANGLES, componentPointer->GetModel().indexBufferData.size(), GL_UNSIGNED_INT, (void*)0);
        } else {
            std::cerr << "No ModelComponent" << std::endl;
        }
    }
}
void RendererSystem::DrawChunk() {
    std::vector<glm::vec3> vertexPositions;
    std::vector<glm::vec3> vertexPositions2;
    std::vector<glm::vec3> vertexPositions3;

    std::vector<GLuint> indexBufferData;
    std::vector<GLuint> indexBufferData2;
    std::vector<GLuint> indexBufferData3;

    Transform transform1;
    Transform transform2;
    Transform transform3;

    vertexPositions = {
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
    vertexPositions2 = {
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
    vertexPositions3 = {
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

    indexBufferData = {
        0,1,2, 3,2,1, // FrontFace
        0,4,5, 1,0,5, // BottomFace
        6,7,4, 7,5,4, // BackFace
        6,2,7, 3,7,2, // TopFace
        3,1,7, 5,7,1, // RightFace
        6,4,2, 0,2,4, // LeftFace
    };
    indexBufferData2 = {
        0,1,2, 3,2,1, // FrontFace
        0,4,5, 1,0,5, // BottomFace
        6,7,4, 7,5,4, // BackFace
        6,2,7, 3,7,2, // TopFace
        3,1,7, 5,7,1, // RightFace
        6,4,2, 0,2,4, // LeftFace
    };
    indexBufferData3 = {
        0,1,2, 3,2,1, // FrontFace
        0,4,5, 1,0,5, // BottomFace
        6,7,4, 7,5,4, // BackFace
        6,2,7, 3,7,2, // TopFace
        3,1,7, 5,7,1, // RightFace
        6,4,2, 0,2,4, // LeftFace
    };

    utility::MeshTranslate(transform1, glm::vec3(0.0f, 2.0f, 0.0f));
    utility::MeshTranslate(transform2, glm::vec3(0.0f, 0.0f, 0.0f));
    utility::MeshTranslate(transform3, glm::vec3(1.0f, 0.0f, 0.0f));

    std::vector<glm::vec3> vertexPositionsCombined;
    std::vector<glm::vec3> textureCoordinateCombined;
    std::vector<GLuint> indexBufferCombined;
    Transform transformFinal;

    utility::MeshTranslate(transformFinal, glm::vec3(0.0f, 0.0f, -2.0f));

    textureCoordinateCombined.insert(textureCoordinateCombined.end(), vertexPositions.begin(), vertexPositions.end());
    textureCoordinateCombined.insert(textureCoordinateCombined.end(), vertexPositions.begin(), vertexPositions.end());
    textureCoordinateCombined.insert(textureCoordinateCombined.end(), vertexPositions.begin(), vertexPositions.end());

    for (size_t i = 0; i < vertexPositions.size(); i++) {
        glm::vec3 vertex = glm::vec3(transform1.mModelMatrix * glm::vec4(vertexPositions[i], 1.0f));
        
        vertexPositionsCombined.push_back(vertex);

    }

    for (size_t i = 0; i < vertexPositions2.size(); i++) {

        glm::vec3 vertex = glm::vec3(transform2.mModelMatrix * glm::vec4(vertexPositions2[i], 1.0f));
        
        vertexPositionsCombined.push_back(vertex);

    }
    for (size_t i = 0; i < vertexPositions3.size(); i++) {
        glm::vec3 vertex = glm::vec3(transform3.mModelMatrix * glm::vec4(vertexPositions3[i], 1.0f));
        
        vertexPositionsCombined.push_back(vertex);

    }

    indexBufferCombined.insert(indexBufferCombined.end(), indexBufferData.begin(), indexBufferData.end());
    
    GLuint offset = vertexPositions.size();
    for (size_t i = 0; i < indexBufferData2.size(); i++) {
        indexBufferCombined.push_back(indexBufferData2[i] + offset);
    }
    for (size_t i = 0; i < indexBufferData3.size(); i++) {
        indexBufferCombined.push_back(indexBufferData3[i] + (offset*2));
    }
    
    std::vector<GLuint> textureIDs;

    for (size_t i = 0; i < vertexPositions.size(); ++i) {
        textureIDs.push_back(0);
    }
    for (size_t i = 0; i < vertexPositions2.size(); ++i) {
        textureIDs.push_back(1); 
    }
    for (size_t i = 0; i < vertexPositions3.size(); ++i) {
        textureIDs.push_back(0); 
    }

    std::vector<GLuint64> textureHandles = {4294969856, 4294969857};

    GLuint vao, vbo, ebo, texId, textures, textureCoords;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Start generating VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexPositionsCombined.size()*sizeof(glm::vec3), vertexPositionsCombined.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferCombined.size()*sizeof(GLuint), indexBufferCombined.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &textures);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, textures);
    glBufferData(GL_SHADER_STORAGE_BUFFER, textureHandles.size() * sizeof(GLuint64), textureHandles.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, textures);
    
    glGenBuffers(1, &textureCoords);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoords);
    glBufferData(GL_ARRAY_BUFFER, textureCoordinateCombined.size() * sizeof(glm::vec3), textureCoordinateCombined.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)0);

    glGenBuffers(1, &texId);
    glBindBuffer(GL_ARRAY_BUFFER, texId);
    glBufferData(GL_ARRAY_BUFFER, textureIDs.size() * sizeof(GLuint), textureIDs.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    
    // Draw
    glUseProgram(mGraphicsApp->mGraphicsPipeline);

    GLint uModelMatrixLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uModelMatrix");
    glUniformMatrix4fv(uModelMatrixLocation, 1, false, &transformFinal.mModelMatrix[0][0]);

    glm::mat4 view = mGraphicsApp->mCamera.GetViewMatrix();
    GLint uViewLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uViewMatrix");
    glUniformMatrix4fv(uViewLocation, 1, false, &view[0][0]);

    glm::mat4 perspective = mGraphicsApp->mCamera.GetProjectionMatrix();
    GLint uProjectionLocation = shader::FindUniformLocation(mGraphicsApp->mGraphicsPipeline, "uProjectionMatrix");
    glUniformMatrix4fv(uProjectionLocation, 1, false, &perspective[0][0]);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexBufferCombined.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}
void RendererSystem::AddGraphicsApp(std::shared_ptr<GraphicsApp> graphicsApp) {
    mGraphicsApp = graphicsApp;
}

RendererSystem& RendererSystem::GetInstance() {
    static RendererSystem sInstance;
    return sInstance;
}