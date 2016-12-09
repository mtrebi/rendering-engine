#include "Mesh.h"
#include <string>

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    this->initialize();
}

void Mesh::initialize() {
    // Generate ids
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    // Bind
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    // Buffer data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof (Vertex), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (GLuint), &indices[0], GL_STATIC_DRAW);

    // Setup for drawing
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex),
            (GLvoid*) 0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex),
            (GLvoid*) offsetof(Vertex, normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex),
            (GLvoid*) offsetof(Vertex, texCoords));

    // Unbind
    glBindVertexArray(0);
}

void Mesh::draw(Shader shader) {
    GLuint diffuseCount = 1;
    GLuint specularCount = 1;

    // Assign textures dynamically
    for (int i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);

        Texture currentTexture = textures[i];

        std::string texture_suffix = "texture";
        switch (currentTexture.type) {
            case Texture::DIFFUSE:
                texture_suffix += "_diffuse" + std::to_string(diffuseCount);
                ++diffuseCount;
                break;
            case Texture::SPECULAR:
                texture_suffix += "_specular" + std::to_string(specularCount);
                ++specularCount;
                break;
        }
        
        glUniform1i(glGetUniformLocation(shader.Program, ("material." + texture_suffix).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}