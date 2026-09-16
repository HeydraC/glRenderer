#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include "vertex.h"

struct Mesh {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    int vertexCount = 0;

    std::vector<Vertex> vertices;
    glm::mat4 model = {{1, 0, 0, 0},
                       {0, 1, 0, 0},
                       {0, 0, 1, 0},
                       {0, 0, 0, 1}};
    glm::vec4 color;

    Mesh(const float*, size_t, int);

    void update(const float*, size_t, int);

    void draw();

    void destroy();

private:
    GLuint createVBO(const float*, size_t, GLenum = GL_STATIC_DRAW);
    GLuint createVAO();
};