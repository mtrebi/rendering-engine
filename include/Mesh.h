#include <glm/glm.hpp>
#include <vector>
#include "shaders/Shader.h"
#include <assimp/types.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    GLuint id;

    enum Type {
        DIFFUSE, SPECULAR
    } type;
    aiString path; // Store path of texture to avoid loading it more than once
};

struct Material {
    float shininess;
};

class Mesh {
public:
    /* Mesh Data */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    Material material; // TODO add more materials
private:
    /* Render Data */
    GLuint m_VAO, // Stores data
    m_VBO, // Data configuration (enable/disable attr)
    m_EBO; // Data indices (order)

public:
    Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures, Material material);
    void draw(Shader shader);
private:
    // Initialize buffers
    void initialize();

};