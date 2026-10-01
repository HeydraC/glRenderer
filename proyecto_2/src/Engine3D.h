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
#include "mesh.h"

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
    glm::vec3 pos    = glm::vec3(0.0f, 4.0f, 12.0f);
    glm::vec3 target = glm::vec3(0.0f, 2.0f, 1.0f); 
    glm::vec3 up     = glm::vec3(0.0f, 1.0f, 0.0f);
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

    std::vector<Mesh> meshes;

    ImGuiContext* context;

    float bColor[3] = {0.1f, 0.1f,0.1f};

    void getUniformLocations();

    void createGLFWwindow();

    void imguiInit();

    void processShaders();

    void frameCount();

    void checkKeyboard();

    void drawGUI();

public:
    Engine3D(int = 800, int = 600);
    ~Engine3D();

    bool closedWindow();

    void run();
};