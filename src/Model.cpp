#include "Model.h"
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GLint TextureFromFile(const char* path, string directory);

Model::Model(GLchar* path) {
    loadModel(path);
}

void Model::draw(Shader shader) {
    for (auto& mesh : m_meshes) {
        mesh.draw(shader);
    }
}

void Model::loadModel(const std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        exit(-1);
    }
    m_directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process all meshes of the node
    for (GLuint i = 0; i < node->mNumMeshes; ++i) {

        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    // Process all children nodes
    for (GLuint i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    Material mtl;
    // Process vertices
    for (GLuint i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;

        // Positions
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        // Normals
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        // Textures
        if (mesh->mTextureCoords) {
            // Has texture coords
            aiVector3D * vector = mesh->mTextureCoords[0];
            if (vector){
                // Has components
                vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
            }else {
                vertex.texCoords.x = 0.0f;
                vertex.texCoords.y = 0.0f;
            }
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (GLuint i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process textures
    if (mesh->mMaterialIndex >= 0) { 
        // Has material
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    
        //TODO: Get normal!        
                  
        aiColor4D Ka;
        aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &Ka);
        mtl.Ka = glm::vec3(Ka.r, Ka.g, Ka.b);
        
        aiColor4D Kd;
        aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &Kd);
        mtl.Kd = glm::vec3(Kd.r, Kd.g, Kd.b);
        
        aiColor4D Ks;
        aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &Ks);
        mtl.Ks = glm::vec3(Ks.r, Ks.g, Ks.b);
        
        float shininess = 0.0f;
        unsigned int max;
        aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max);
        mtl.shininess = shininess/4 ; 
        
        float transparency = 0.0f;
        aiGetMaterialFloatArray(material, AI_MATKEY_COLOR_TRANSPARENT, &transparency, &max);
        mtl.shininess = transparency; 

    }
    
    return Mesh(vertices, indices, textures, mtl);

}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, Texture::Type textureType) {
    std::vector<Texture> textures;
    for (GLuint i = 0; i < material->GetTextureCount(type); ++i) {
        aiString str;
        material->GetTexture(type, i, &str);
        GLboolean skip = false;
        for (GLuint j = 0; j < m_textures_loaded.size(); ++j) {
            if (m_textures_loaded[j].path == str) {
                textures.push_back(m_textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), m_directory);
            texture.type = textureType;
            texture.path = str;
            textures.push_back(texture);
            m_textures_loaded.push_back(texture);
        }
    }
    return textures;
}

GLint TextureFromFile(const char* path, string directory) {
    //Generate texture ID and load texture data 
    string filename = string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}