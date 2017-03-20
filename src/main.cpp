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

const GLchar* vs_path = "src/shaders/default.vs";
const GLchar* fs_path = "src/shaders/default.fs";


// Forward declarations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);





// The MAIN function, from here we start the application and run the game loop
int main(){
  
  
  glfwInit();
  // OpenGL version 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow * window = glfwCreateWindow(800, 600, "Tutorial_1", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  // Initialize GLEW pointers to OpenGL functions
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  // Setup Vertex Data
  GLfloat vertices[] = {
    0.5f,  0.5f, 0.0f,  // Top Right
    0.5f, -0.5f, 0.0f,  // Bottom Right
    -0.5f, -0.5f, 0.0f,  // Bottom Left
    -0.5f,  0.5f, 0.0f   // Top Left 
  };
  GLuint indices[] = {  // Note that we start from 0!
    0, 1, 3,   // First Triangle
    1, 2, 3    // Second Triangle
  };

  // Setup Vertex Buffer Object
  GLuint VBO, VAO, EBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // 0. Copy our vertices array in a buffer for OpenGL to use
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 1. Then set the vertex attributes pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);


  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // 2. Setup Shaders
  Shader shader(vs_path, fs_path);
  shader.Use();

  // Setup OpenGL viewport with Window sizes
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  // This info is used to transform from NDC to Screen Coordinates through Viewport Transform
  glViewport(0, 0, width, height);

  while (!glfwWindowShouldClose(window)) {
    // Handle user input (keyboard, mouse...)
    glfwPollEvents();

    // Rendering commands go here...
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    // Swap color buffers (uses double buffering -> front and back)
    glfwSwapBuffers(window);
  }

  // Delete all resources
  glfwTerminate();
  return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  // When a user presses the escape key, we set the WindowShouldClose property to true, 
  // closing the application
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}