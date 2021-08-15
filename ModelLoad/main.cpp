#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"
#include "shader.hpp"
#include "stb_image.h"
#include "model.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, Shader &myShader);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mod);
unsigned int loadTexture(const char *path);
void TextureSet(Shader &myShader);
void VertexSet();
void setPointLight(Shader &myShader, const std::string &name);

const unsigned int WIDTG = 800;
const unsigned int HEIGHT = 600;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
float lastX = WIDTG/2, lastY = HEIGHT/2;
bool firstMouse = true;

int isShow = 1;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 lightPosition = glm::vec3( 0.7f,  0.2f,  1.0f);
float vertices[] = {
    // positions          // normals           // texture coords
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

int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(WIDTG, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowPos(window, 300, 200);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

     stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);

    // build and compile shaders
    // -------------------------
    Shader myShader("../objshader.vs.glsl", "../objshader.fs.glsl");
    Shader lightShader("../lightshader.vs.glsl", "../lightshader.fs.glsl");

    unsigned int VBO, lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // load models
    // -----------
    //Model myModel("../models/armor/source/NeutralIdle.fbx");
    //Model myModel("../models/bird/source/falcon-concrete.fbx");
    //Model myModel("../models/0714_资产对接文件/底板/SM_Terrain_.FBX");
    Model myModel("../models/nanosuit/nanosuit.obj");
    

    while(!glfwWindowShouldClose(window)){
        processInput(window, myShader);
        
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1.f*WIDTG/HEIGHT, 0.1f, 100.f);
        myShader.use();

        myShader.setMat4("projection", projection);
        myShader.setMat4("view", view);

        myShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        myShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        myShader.setVec3("dirLight.diffuse", 1.f, 1.f, 1.f);
        myShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        float radius = sqrt(1.0);
        float speed = 100.f;
        lightPosition = glm::vec3(radius*cos(glm::radians(currentFrame*speed)), lightPosition.y, radius*sin(glm::radians(currentFrame*speed)));
        //std::cout << currentFrame << std::endl;
        myShader.setVec3("viewPos", camera.Position);
        myShader.setVec3("pointLight.position", lightPosition);
        myShader.setVec3("pointLight.ambient", 0.5f, 0.5f, 0.5f);
        myShader.setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
        myShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
        myShader.setFloat("pointLight.constant", 1.0f);
        myShader.setFloat("pointLight.linear", 0.09);
        myShader.setFloat("pointLight.quadratic", 0.032);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.01f));	// it's a bit too big for our scene, so scale it down
        myShader.setMat4("model", model);
        myModel.Draw(myShader);
        //Mountain.Draw(myShader);

        lightShader.use();
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        glBindVertexArray(lightVAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.1f));
        lightShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mod){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        isShow = !isShow;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float dx = xpos-lastX;
    float dy = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05f;

    camera.ProcessMouseMovement(dx, dy);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window, Shader &myShader){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

unsigned int loadTexture(char const * path){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        //std::cout << nrComponents << std::endl;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}