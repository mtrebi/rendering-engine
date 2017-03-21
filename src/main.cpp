#include <iostream>

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
#include "Model.h"

// Function prototypes
GLFWwindow* initialize(const GLuint width, const GLuint height);

void terminate();
void clearBuffers();
void calculateFrameTime();
void calculateCameraMovement();
void setupData();
void setupOpenGLFlags();
void setupProjectionViewMatrix(Shader shader);
void setupModelMatrix(Shader shader, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 translate = glm::vec3(1.0f, 1.0f, 1.0f));

// Callbacks
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Attributes
const GLuint SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400,        // Mouse last X position
lastY = 300;        // Mouse last Y position
GLfloat deltaTime = 0.0f,   // Time between current frame and last frame
lastFrame = 0.0f;   // Time of last frame
bool firstMouse = false;

const GLchar* VS_PATH = "src/shaders/default.vs";
const GLchar* FS_PATH = "src/shaders/default.fs";

const GLchar* LIGHT_VS_PATH = "src/shaders/light_shader.vs";
const GLchar* LIGHT_FS_PATH = "src/shaders/light_shader.fs";


GLchar* CUBE_PATH = "assets/models/box/box.obj";

GLuint light_VAO, cube_VAO;
GLuint diffuseMap, specularMap;



glm::vec3 light_position = glm::vec3(1.0f, 1.0f, 2.0f);

//GLuint VBO, lightVAO, cubeVAO, diffuseTexture, specularTexture, uboMatrices;

// The MAIN function, from here we start the application and run the game loop
int main() {

  GLFWwindow* window = initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

  setupOpenGLFlags();

  setupData();

  Shader default_shader = Shader(VS_PATH, FS_PATH);
  Shader light_shader = Shader(LIGHT_VS_PATH, LIGHT_FS_PATH);

  // Game loop
  while (!glfwWindowShouldClose(window))
  {
    // Set frame time
    calculateFrameTime();
    glfwPollEvents();
    calculateCameraMovement();
    clearBuffers();

    light_shader.Use();

    setupProjectionViewMatrix(light_shader);
    setupModelMatrix(light_shader, glm::vec3(0.2f), light_position);

    ////////////////////////
    //light_position.x = sin(glfwGetTime());
    //light_position.y = cos(glfwGetTime());

    ////////////////////////


    glBindVertexArray(light_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    default_shader.Use();

    ////////////////////////
    glUniform3f(glGetUniformLocation(default_shader.Program, "camera_position"), camera.Position.x, camera.Position.y, camera.Position.z);
    
    glUniform3f(glGetUniformLocation(default_shader.Program, "spot_light.position"), camera.Position.x, camera.Position.y, camera.Position.z);
    glUniform3f(glGetUniformLocation(default_shader.Program, "spot_light.direction"), camera.Front.x, camera.Front.y, camera.Front.z);

    glUniform3f(glGetUniformLocation(default_shader.Program, "spot_light.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(default_shader.Program, "spot_light.diffuse"), 0.4f, 0.4f, 0.4f);
    glUniform3f(glGetUniformLocation(default_shader.Program, "spot_light.specular"), 0.4f, 0.4f, 0.4f);
    glUniform1f(glGetUniformLocation(default_shader.Program, "spot_light.cutOff"), glm::cos(glm::radians(12.5f)));
    glUniform1f(glGetUniformLocation(default_shader.Program, "spot_light.outerCutOff"), glm::cos(glm::radians(17.5f)));

    /*
    glUniform3f(glGetUniformLocation(default_shader.Program, "point_light.position"), light_position.x, light_position.y, light_position.z);
    glUniform3f(glGetUniformLocation(default_shader.Program, "point_light.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(default_shader.Program, "point_light.diffuse"), 0.4f, 0.4f, 0.4f);
    glUniform3f(glGetUniformLocation(default_shader.Program, "point_light.specular"), 0.4f, 0.4f, 0.4f);
    glUniform1f(glGetUniformLocation(default_shader.Program, "point_light.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(default_shader.Program, "point_light.linear"), 0.09f);
    glUniform1f(glGetUniformLocation(default_shader.Program, "point_light.quadratic"), 0.032f);
    */
    glUniform1i(glGetUniformLocation(default_shader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(default_shader.Program, "material.specular"), 1);
    
    /*
    glUniform3f(glGetUniformLocation(default_shader.Program, "directional_light.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(default_shader.Program, "directional_light.ambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(default_shader.Program, "directional_light.diffuse"), 0.4f, 0.4f, 0.4f);
    glUniform3f(glGetUniformLocation(default_shader.Program, "directional_light.specular"), 0.4f, 0.4f, 0.4f);

    glUniform1i(glGetUniformLocation(default_shader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(default_shader.Program, "material.specular"), 1);
    */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);


    glUniform1f(glGetUniformLocation(default_shader.Program, "material.shininess"), 32.0f);

    ////////////////////////

    setupProjectionViewMatrix(default_shader);
    setupModelMatrix(default_shader);

    glBindVertexArray(cube_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }


  terminate();
  return 0;
}

GLFWwindow* initialize(const GLuint width, const GLuint height) {
  // Init GLFW
  glfwInit();
  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Create a GLFWwindow object that we can use for GLFW's functions
  GLFWwindow* window = glfwCreateWindow(width, height, "rendering-engine", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  // Set the required callback functions
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);

  // Options
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
  glewExperimental = GL_TRUE;
  glewInit();

  // Define the viewport dimensions
  glViewport(0, 0, width, height);

  return window;
}

void terminate() {
  /*
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &VBO);
  */
  glfwTerminate();
}

void setupOpenGLFlags() {
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_STENCIL_TEST);
  //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void clearBuffers() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void calculateFrameTime() {
  GLfloat currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

void calculateCameraMovement() {
  // Camera controls
  if (keys[GLFW_KEY_W])
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (keys[GLFW_KEY_S])
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (keys[GLFW_KEY_A])
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (keys[GLFW_KEY_D])
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

void setupData() {
  /*
  glGenBuffers(1, &uboMatrices);
  glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);
  */

  GLfloat vertices[] = {
    // Positions           // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
  };

  GLuint light_VBO;
  glGenBuffers(1, &light_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, light_VBO);
  
  glGenVertexArrays(1, &light_VAO);
  glBindVertexArray(light_VAO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);


  GLuint cube_VBO;
  glGenBuffers(1, &cube_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);

  glGenVertexArrays(1, &cube_VAO);
  glBindVertexArray(cube_VAO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);



  glGenTextures(1, &diffuseMap);
  glGenTextures(1, &specularMap);
  int width, height;
  unsigned char* image;
  // Diffuse map
  image = SOIL_load_image("assets/textures/container2.png", &width, &height, 0, SOIL_LOAD_RGB);
  glBindTexture(GL_TEXTURE_2D, diffuseMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  // Specular map
  image = SOIL_load_image("assets/textures/container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB);
  glBindTexture(GL_TEXTURE_2D, specularMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void setupProjectionViewMatrix(Shader shader) {
  /*
  glUniformBlockBinding(shader.Program, glGetUniformBlockIndex(shader.Program, "Matrices"), 0);

  glm::mat4 projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
  glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glm::mat4 view = camera.GetViewMatrix();
  glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  */
  glm::mat4 projection;
  projection = glm::perspective(45.0f, (float) SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  glm::mat4 view = camera.GetViewMatrix();
  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
}

void setupModelMatrix(Shader shader, glm::vec3 scale, glm::vec3 translate) {
  glm::mat4 model = glm::mat4();
  model = glm::translate(model, translate);
  model = glm::scale(model, scale);
  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key >= 0 && key < 1024)
  {
    if (action == GLFW_PRESS)
      keys[key] = true;
    else if (action == GLFW_RELEASE)
      keys[key] = false;
  }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  GLfloat xoffset = xpos - lastX;
  GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}