#include "Mesh.h"
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model {
private:
	std::vector<Mesh> m_meshes;
	std::vector<Texture> m_textures_loaded;
	std::string m_directory;

  glm::mat4 m_model;
public:
	Model(GLchar* path);
	void draw(Shader shader);

  void translate(const glm::vec3& translation);
  void scale(const glm::vec3& scale);
  void rotate(const glm::vec3& axis, const float degrees);

private:
	void loadModel(const std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, Texture::Type textureType);

        GLint TextureFromFile(const char* path, std::string directory, GLint level);

};