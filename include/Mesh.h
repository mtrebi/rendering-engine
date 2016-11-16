#include <glm/glm.hpp>
#include <vector>
#include "shaders/Shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	GLuint id;
	enum { DIFFUSE, SPECULAR } type;
};

class Mesh {
public:
	/* Mesh Data */
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

private:
	/* Render Data */
	GLuint 	m_VAO, 	// Stores data
			m_VBO, 	// Data configuration (enable/disable attr)
			m_EBO;	// Data indices (order)

public:
	Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures);
	void draw(Shader shader);
private:
	// Initialize buffers
	void initialize();

};