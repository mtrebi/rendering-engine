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
GLuint loadTexture(GLchar* path);
void terminate();
void clearBuffers();
void calculateFrameTime();
void calculateCameraMovement();
void setupData();
void setupOpenGLFlags();
void setupPhongVariables(Shader shader);
void setupProjectionViewMatrix(Shader shader);
void setupModelMatrix(Shader shader, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 translate = glm::vec3(1.0f, 1.0f, 1.0f));

void moveLight();
// Callbacks
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Attributes
const GLuint screenWidth = 800, screenHeight = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); 
bool keys[1024];
GLfloat lastX = 400,        // Mouse last X position
        lastY = 300;        // Mouse last Y position
GLfloat deltaTime = 0.0f,   // Time between current frame and last frame
        lastFrame = 0.0f;   // Time of last frame
bool firstMouse = false;

GLchar* lightVSPath = "../src/shaders/light_shader.vs";
GLchar* lightFSPath = "../src/shaders/light_shader.fs";

GLchar* phongVSPath = "../src/shaders/phong_texture.vs";
GLchar* phongFSPath = "../src/shaders/phong_texture.fs";

GLchar* cubePath = "../assets/models/box/box.obj";
GLchar* dragonPath = "../assets/models/dragon_vn/dragon.obj";
GLchar* nanosuitPath = "../assets/models/nanosuit/nanosuit.obj";

GLuint VBO, lightVAO, cubeVAO, diffuseTexture, specularTexture, uboMatrices;

struct PointLight {
    glm::vec3 position;
    
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    
    GLfloat Kc;
    GLfloat Kl;
    GLfloat Kq;
};

struct DirectionalLight {
    glm::vec3 direction;
    
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    
    float Krim;
};

PointLight pointLight = {glm::vec3(1.5f, 4.0f, 1.5f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(0.5f), 1.0f, 0.09f, 0.032f };
DirectionalLight directionalLight = {glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(0.5f), 2.0f};

// The MAIN function, from here we start the application and run the game loop
int main(){

    GLFWwindow* window = initialize(screenWidth, screenHeight);
    
    setupOpenGLFlags();
    
    setupData();
    
    Shader lightShader = Shader(lightVSPath, lightFSPath);
    Shader phongShader = Shader(phongVSPath, phongFSPath);
    
    Model cube = Model(cubePath);
    Model dragon = Model(dragonPath);
    Model nanosuit = Model(nanosuitPath);

    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        // Set frame time
        calculateFrameTime();
        glfwPollEvents();
        calculateCameraMovement();
        clearBuffers();
             
        //moveLight();
        
        lightShader.Use();
        setupProjectionViewMatrix(lightShader);
        setupModelMatrix(lightShader, glm::vec3(0.2f), pointLight.position);
        //cube.draw(lightShader);
               
        phongShader.Use();
        setupPhongVariables(phongShader);
        setupProjectionViewMatrix(phongShader);
        setupModelMatrix(phongShader, glm::vec3(0.3f));
        //cube.draw(phongShader);
        //dragon.draw(phongShader);
        nanosuit.draw(phongShader);
        glfwSwapBuffers(window);
    }
    terminate();
    return 0;
}

GLFWwindow* initialize(const GLuint width, const GLuint height){
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
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
    glViewport(0, 0, width, height);
    
    return window;
}

void terminate(){
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
}

void setupOpenGLFlags(){
    glEnable(GL_DEPTH_TEST);    
    //glEnable(GL_STENCIL_TEST);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void clearBuffers(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void calculateFrameTime(){
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void moveLight(){
    GLfloat currentFrame = glfwGetTime();
    pointLight.position.x = 1.0f + (sin(glfwGetTime())) * 2.0f;
    pointLight.position.y = 1.0f + (cos(glfwGetTime())) * 2.0f;

}

void calculateCameraMovement() {
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

void setupData() { 
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices); 
}

void setupPhongVariables(Shader shader){
    glUniform3f(glGetUniformLocation(shader.Program, "directionalLight.direction"), directionalLight.direction.x, directionalLight.direction.y, directionalLight.direction.z);
    glUniform3f(glGetUniformLocation(shader.Program, "directionalLight.Ka"),  directionalLight.Ka.x, directionalLight.Ka.y, directionalLight.Ka.z);
    glUniform3f(glGetUniformLocation(shader.Program, "directionalLight.Kd"),  directionalLight.Kd.x, directionalLight.Kd.y, directionalLight.Kd.z);
    glUniform3f(glGetUniformLocation(shader.Program, "directionalLight.Ks"),  directionalLight.Ks.x, directionalLight.Ks.y, directionalLight.Ks.z);
    glUniform1f(glGetUniformLocation(shader.Program, "directionalLight.Krim"),  directionalLight.Krim);
      
    glUniform3f(glGetUniformLocation(shader.Program, "pointLight.position"), pointLight.position.x, pointLight.position.y, pointLight.position.z);
    glUniform3f(glGetUniformLocation(shader.Program, "pointLight.Ka"),  pointLight.Ka.x, pointLight.Ka.y, pointLight.Ka.z);
    glUniform3f(glGetUniformLocation(shader.Program, "pointLight.Kd"),  pointLight.Kd.x, pointLight.Kd.y, pointLight.Kd.z);
    glUniform3f(glGetUniformLocation(shader.Program, "pointLight.Ks"),  pointLight.Ks.x, pointLight.Ks.y, pointLight.Ks.z);

    glUniform1f(glGetUniformLocation(shader.Program, "pointLight.Kc"),  pointLight.Kc);
    glUniform1f(glGetUniformLocation(shader.Program, "pointLight.Kl"),  pointLight.Kl);
    glUniform1f(glGetUniformLocation(shader.Program, "pointLight.Kq"),  pointLight.Kq);

    
   glUniform3f(glGetUniformLocation(shader.Program, "vCameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);


}

void setupProjectionViewMatrix(Shader shader) {
    glUniformBlockBinding(shader.Program, glGetUniformBlockIndex(shader.Program, "Matrices"), 0);
    
    glm::mat4 projection = glm::perspective(45.0f, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0); 
    
    glm::mat4 view = camera.GetViewMatrix();	       
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0); 
}

void setupModelMatrix(Shader shader, glm::vec3 scale, glm::vec3 translate){
    glm::mat4 model = glm::mat4();
    model = glm::translate(model, translate);
    model = glm::scale(model, scale);
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "Model"), 1, GL_FALSE, glm::value_ptr(model));
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path)
{
    //Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);    

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;

}