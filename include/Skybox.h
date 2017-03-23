#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Simple OpenGL Image Library
#include <SOIL/SOIL.h>

// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "shaders/Shader.h"
#include "Camera.h"

#include <string>
#include <array>

class Skybox {
private:
  GLuint m_VAO, m_id;
  std::array<std::string, 6> m_textures = std::array<std::string, 6>();
  Camera* m_camera;
public:
  Skybox(Camera* camera, const std::string base_path);
  Skybox(Camera* camera, const std::string right, const std::string left, const std::string top, const std::string bottom, const std::string back, const std::string front);

  void setup_skybox_uniform(Shader& shader, const GLuint texture_unit = 0);
  void draw(Shader& shader);

private:
  void load_textures();
  void load_VAO();
  void setup_matrices_uniform(Shader &shader);
};