#include "Skybox.h"

Skybox::Skybox(Camera* camera, const std::string base_path)
  : m_camera(camera) {
  m_textures[0] = base_path + "right.jpg";
  m_textures[1] = base_path + "left.jpg";
  m_textures[2] = base_path + "top.jpg";
  m_textures[3] = base_path + "bottom.jpg";
  m_textures[4] = base_path + "back.jpg";
  m_textures[5] = base_path + "front.jpg";

  load_VAO();
  load_textures();
}

Skybox::Skybox(Camera* camera, const std::string right, const std::string left, const std::string top, const std::string bottom, const std::string back, const std::string front)
  : m_camera(camera) {
  m_textures[0] = right;
  m_textures[1] = left;
  m_textures[2] = top;
  m_textures[3] = bottom;
  m_textures[4] = back;
  m_textures[5] = front;

  load_VAO();
  load_textures();
}

void Skybox::setup_skybox_uniform(Shader& shader, const GLuint texture_unit) {
  shader.Use();
  glActiveTexture(GL_TEXTURE0 + texture_unit);      // Activate texture uni
  glUniform1i(glGetUniformLocation(shader.Program, "skybox"), 0); // Bind skybox to texture unit
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);   // Bind cubemap to active texture 

}

void Skybox::draw(Shader& shader) {
  glDepthMask(GL_FALSE);
  shader.Use();

  glm::mat4 view = glm::mat4(glm::mat3(m_camera->GetViewMatrix()));
  glm::mat4 projection = m_camera->GetProjectionMatrix();

  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  // Setup texture
  setup_skybox_uniform(shader);

  // Draw
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Reset
  glBindVertexArray(0);
  glDepthMask(GL_TRUE);
}

void Skybox::load_textures() {
  glGenTextures(1, &m_id);
  glActiveTexture(GL_TEXTURE0);

  int width, height;
  unsigned char* image;

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
  for (GLuint i = 0; i < 6; i++)
  {
    image = SOIL_load_image(m_textures[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    if (!image) {
      std::cerr << "SOIL::LOAD CUBEMAP FAILED" << std::endl;
      exit(-1);
    }
    glTexImage2D(
      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
      GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
    );
    SOIL_free_image_data(image);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::load_VAO() {
  GLuint VBO;

  const GLfloat vertices[] = {
    // Positions          
  -1.0f,  1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

  -1.0f,  1.0f, -1.0f,
  1.0f,  1.0f, -1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
  1.0f, -1.0f, -1.0f,
  1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
  1.0f, -1.0f,  1.0f
  };

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);
  
  glBindVertexArray(0);
}