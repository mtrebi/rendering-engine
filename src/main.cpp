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


  // Setup OpenGL viewport with Window sizes
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  // This info is used to transform from NDC to Screen Coordinates through Viewport Transform
  glViewport(0, 0, width, height);

  while (!glfwWindowShouldClose(window)) {
    // Handle user input (keyboard, mouse...)
    glfwPollEvents();

    // Rendering commands go here...
    glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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