#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "Model.h"

#ifndef SRC
    #define SRC "."
#endif

struct Shader{
    GLuint program;

    GLint model;
    GLint view;
    GLint projection;
    GLint objectColor;
    GLint lightDir;
    GLint lightColor;
    GLint ambientLight;
};

struct Camera {
    glm::vec3 pos   = glm::vec3(0.0f, 4.0f, 12.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

    float yaw   = -90.0f; //Horizontal
    float pitch = -10.3f; //Vertical

    void updateVectors() {
        glm::vec3 dir;
        dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        dir.y = sin(glm::radians(pitch));
        dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        front = glm::normalize(dir);
        right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        up    = glm::normalize(glm::cross(right, front));
    }
};

class Engine3D{
private:
    GLFWwindow* window;
    int width, height;
    float aspect;
    float lastFrame, delta, timeElapsed;

    glm::mat4 view, projection;
    glm::vec3 lightDir, lightColor, ambientLight;

    glm::vec4 backgroundColor;
    
    Shader shader;

    Camera cam;

    float lastMouseX = 0.0f, lastMouseY = 0.0f;
    float sensitivity = 0.1f;
    bool mouseCamera = false;

    std::vector<Model> models;

    ImGuiContext* context;

    float moveSpeed = 5.0f;
    float rotSpeed = 75.0f;

    bool depthTest = true, backFaceCulling = true;

    float bColor[3] = {0.1f, 0.1f,0.1f};
    float mColor[4];
    float sColor[4];

    int selectedModel = -1;
    int selectedMesh = -1;

    void getUniformLocations();

    void createGLFWwindow();

    void imguiInit();

    void processShaders();

    static void keyCallback(GLFWwindow*, int, int, int, int);

    static void mouseButtonCallback(GLFWwindow*, int, int, int);
    
    static void cursorPosCallback(GLFWwindow*, double, double);

    static void windowSizeCallback(GLFWwindow*, int, int);

    void onKeyDown(int);

    void onKeyUp(int);

    glm::vec2 getMousePosition();

    void onMouseButtonDown(int, float, float);

    void onMouseButtonUp(int, float, float);

    void onMouseMove(float, float);

    void checkKeyboard();

    void frameCount();

    void drawGUI();

    void selectModel(float, float);

    void selectMesh(float, float);

    void saveState(char*);

    void loadState(char*);

public:
    Engine3D(int = 800, int = 600);
    ~Engine3D();

    bool closedWindow();

    void run();
};