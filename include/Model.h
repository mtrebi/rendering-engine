#include "Mesh.h"
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
private:
	std::vector<Mesh> m_meshes;
	std::vector<Texture> m_textures_loaded;
	std::string m_directory;
public:
	Model(GLchar* path);
	void draw(Shader shader);

private:
	void loadModel(const std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, Texture::Type textureType);
};