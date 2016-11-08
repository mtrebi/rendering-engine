// GLEW: Include glew before glfw3 because it defines the impl. of OpenGL
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>

/**
    Initialize Glew
*/
void initializeGlew(){
    // Enable experimental features
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK){
        std::cerr << "Failed to inicitalize GLEW" << std::endl;
    }
}

int main(void)
{
    // Initialize glfw (memory allocation)
    glfwInit();
    // Set OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Set working mode to core-profile (disable legacy calls to immediate-mode)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Window Title", nullptr, nullptr);
    if (window == nullptr){
        std::cerr << "Failted to create GLFW window" << std::endl;
        // Destroys all dynamic allocated memory for GLFW
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
       initializeGlew();

    // Tell openGl the size of the rendering display
    int width, height;
    // Get the window parameters
    glfwGetFramebufferSize(window, &width, &height);
    // Set viewport at location 0,0
    glViewport(0, 0, width, height);

    // Run game loop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        // Swap front and back buffer (Double-buffer technique)
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
