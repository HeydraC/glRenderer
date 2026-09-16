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
#include <sstream>
#include "mesh.h"

#ifndef SHADER_DIR
    #define SHADER_DIR "./shaders/"
#endif

GLFWwindow* createGlfwWindow();
GLuint processShaders();