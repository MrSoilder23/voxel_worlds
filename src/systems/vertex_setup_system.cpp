#include "./systems/vertex_setup_system.hpp"

void VertexSetupSystem::update(bismuth::Registry& registry) {
    auto& meshPool      = registry.getComponentPool<MeshComponent>();
    auto& modelDenseIDs = meshPool.getDenseEntities();

    for(auto& entityID : modelDenseIDs) {
        auto& mesh = meshPool.getComponent(entityID);

        if(!mesh.isDirty) {
            continue;
        }

        mesh.isDirty = false;
        
        if (mesh.vertices.empty()) {
            continue;
        }

        if(mesh.VAO == 0) {
            glGenVertexArrays(1, &mesh.VAO);
        }
        glBindVertexArray(mesh.VAO);
        
        if(mesh.VBO == 0) {
            glGenBuffers(1, &mesh.VBO);
        }
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);
        
        if (!mesh.indices.empty()) {
            if(mesh.EBO == 0) {
                glGenBuffers(1, &mesh.EBO);
            }
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), mesh.indices.data(), GL_STATIC_DRAW);
        }

        // Position attribute (location = 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        // Normal attribute (location = 1)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // Texture coordinate attribute (location = 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
        
        // Color attribute (location = 3)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

        glBindVertexArray(0);
    }
}