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

// Properties
const GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
void calculate_times();
void calculate_lightPos();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); 
bool keys[1024];
GLfloat lastX = 400,        // Mouse last X position
        lastY = 300;        // Mouse last Y position
GLfloat deltaTime = 0.0f,   // Time between current frame and last frame
        lastFrame = 0.0f;   // Time of last frame
bool firstMouse = false;

// Light
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);


// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback); 
    glfwSetCursorPosCallback(window, mouse_callback);  

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program
    Shader basicShader("../src/shaders/default.vs", "../src/shaders/default.fs");
    Shader lampShader("../src/shaders/default.vs", "../src/shaders/light.fs");

    // Set up vertex data (and buffer(s)) and attribute pointers
    // Set up vertex data (and buffer(s)) and attribute pointers
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


    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    /****** OBJECT  ******/

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0); // Unbind VAO

    /****** LIGHT ******/

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // We only need to bind to VBO. We don't need to fill it with data because we already did it before
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // We don't care about texture data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Load textures: diffuse map
    
    int width, height;
    unsigned char* image = SOIL_load_image("../assets/textures/container2.png", &width, &height, 0, SOIL_LOAD_RGB); 
    GLuint diffuseMap;
    glGenTextures(1, &diffuseMap);  
    glBindTexture(GL_TEXTURE_2D, diffuseMap);  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Load textures: specular map
    image = SOIL_load_image("../assets/textures/container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB); 
    GLuint specularMap;
    glGenTextures(1, &specularMap);  
    glBindTexture(GL_TEXTURE_2D, specularMap);  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    basicShader.Use();
    // Set material
    glUniform1i(glGetUniformLocation(basicShader.Program, "material.diffuse"),          0);
    glUniform1i(glGetUniformLocation(basicShader.Program, "material.specular"),         1);
    glUniform1f(glGetUniformLocation(basicShader.Program, "material.shininess"),        32.0f);
    // Set static light
    glUniform3f(glGetUniformLocation(basicShader.Program, "directionalLight.direction"),lightDir.x, lightDir.y, lightDir.z);
    glUniform3f(glGetUniformLocation(basicShader.Program, "directionalLight.Ka"),       0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(basicShader.Program, "directionalLight.Kd"),       0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(basicShader.Program, "directionalLight.Ks"),       1.0f, 1.0f, 1.0f);
    // Set others
    glUniform3f(glGetUniformLocation(basicShader.Program, "objectColor"),               1.0f, 0.5f, 0.31f);

    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        // Set frame time
        calculate_times();
        calculate_lightPos();

        // Check and call events
        glfwPollEvents();
        do_movement();

        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get the uniform locations
        basicShader.Use();

        // Dynamic camera pos
        glUniform3f(glGetUniformLocation(basicShader.Program, "viewPos"),               camera.Position.x, camera.Position.y, camera.Position.z);
        // Dynamic lights (point lights)
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[0].position"),pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[0].Ka"),       0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[0].Kd"),       0.8f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[0].Ks"),       1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(basicShader.Program, "pointLights[0].Kc"),       1.0);
        glUniform1f(glGetUniformLocation(basicShader.Program, "pointLights[0].Kl"),       0.09);
        glUniform1f(glGetUniformLocation(basicShader.Program, "pointLights[0].Kq"),       0.032);

        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[1].position"),pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[1].Ka"),       0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[1].Kd"),       0.8f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[1].Ks"),       1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(basicShader.Program, "pointLights[1].Kc"),       1.0);
        glUniform1f(glGetUniformLocation(basicShader.Program, "pointLights[1].Kl"),       0.09);
        glUniform1f(glGetUniformLocation(basicShader.Program, "pointLights[1].Kq"),       0.032);

        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[2].position"),pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[2].Ka"),       0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[2].Kd"),       0.8f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[2].Ks"),       1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(basicShader.Program, "pointLights[2].Kc"),       1.0);
        glUniform1f(glGetUniformLocation(basicShader.Program, "pointLights[2].Kl"),       0.09);
        glUniform1f(glGetUniformLocation(basicShader.Program, "pointLights[2].Kq"),       0.032);

        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[3].position"),pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[3].Ka"),       0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[3].Kd"),       00.8f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(basicShader.Program, "pointLights[3].Ks"),       1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(basicShader.Program, "pointLights[3].Kc"),       1.0);
        glUniform1f(glGetUniformLocation(basicShader.Program, "pointLights[3].Kl"),       0.09);
        glUniform1f(glGetUniformLocation(basicShader.Program, "pointLights[3].Kq"),       0.032);

        glUniform3f(glGetUniformLocation(basicShader.Program, "spotLight.position"),    camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform3f(glGetUniformLocation(basicShader.Program, "spotLight.direction"),    camera.Front.x, camera.Front.y, camera.Front.z);
        glUniform3f(glGetUniformLocation(basicShader.Program, "spotLight.Ka"),       0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(basicShader.Program, "spotLight.Kd"),       1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(basicShader.Program, "spotLight.Ks"),       1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(basicShader.Program, "spotLight.Kc"),       1.0);
        glUniform1f(glGetUniformLocation(basicShader.Program, "spotLight.Kl"),       0.09);
        glUniform1f(glGetUniformLocation(basicShader.Program, "spotLight.Kq"),       0.032);
        glUniform1f(glGetUniformLocation(basicShader.Program, "spotLight.innerCutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(basicShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

        // Create camera transformations
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(basicShader.Program, "model");
        GLint viewLoc  = glGetUniformLocation(basicShader.Program,  "view");
        GLint projLoc  = glGetUniformLocation(basicShader.Program,  "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // Draw the container (using container's vertex attributes)
        glBindVertexArray(VAO);
        glm::mat4 model;

        for(GLuint i = 0; i < 10; i++){
           model = glm::mat4();
           model = glm::translate(model, cubePositions[i]);
           GLfloat angle = 20.0f * i; 
           model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
           glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

           glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        lampShader.Use();
        glBindVertexArray(lightVAO);

        // Get the uniform locations
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc  = glGetUniformLocation(lampShader.Program,  "view");
        projLoc  = glGetUniformLocation(lampShader.Program,  "projection");

        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        for(GLuint i = 0; i < 3; i++){
            model = glm::mat4();
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36); 
        }

        glBindVertexArray(0);
        // Swap the buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void calculate_times(){
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void do_movement() {
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;  
    } 
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(firstMouse)
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

void calculate_lightPos(){


    //lightPos.x = ;
    //lightPos.y = cos(glfwGetTime());
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}