#define TINYOBJLOADER_IMPLEMENTATION
#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
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

struct Camera {
    glm::vec3 pos    = glm::vec3(0.0f, 4.0f, 12.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 1.0f); 
    glm::vec3 up     = glm::vec3(0.0f, 1.0f, 0.0f);
};

void checkKeyboard(glm::mat4&, Camera&, float, GLFWwindow*);
void frameCount(float&, float&, GLFWwindow*);

GLFWwindow* createGlfwWindow();
GLuint processShaders();

glm::mat4 customLookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);
glm::mat4 customPerspective(float fovRadians, float aspect, float zNear, float zFar);