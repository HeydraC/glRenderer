#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <string>
#include "vertex.h"
#include "objLoader.h"

struct Mesh {
    GLuint vao = -1;
    GLuint vbo = -1;
    glm::vec4 objectColor;

    std::vector<Vertex> vertices;
    glm::mat4 model = {{1, 0, 0, 0},
                       {0, 1, 0, 0},
                       {0, 0, 1, 0},
                       {0, 0, 0, 1}};
    glm::vec4 color;

    Mesh(std::string);

    void update(const float*);

    void draw(GLuint, GLint, GLint);

    void destroy();

private:
    GLuint createVBO(std::vector<Vertex>&, GLenum = GL_STATIC_DRAW);
    GLuint createVAO();
};