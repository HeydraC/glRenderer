#pragma once

#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex{
    glm::vec3 pos;
    glm::vec3 normal;
};

struct Mesh {
    GLuint vao = -1;
    GLuint vbo = -1;
    glm::vec4 objectColor{1.0f};

    std::vector<Vertex> vertices;
    glm::mat4 model{1.0f};

    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};

    bool wireframe = false;

    bool showNormals = false;
    GLuint normalVao = 0;
    GLuint normalVbo = 0;
    size_t normalVertexCount = 0;

    bool showVertices = false;

    bool showBoundingBox = false;
    glm::vec3 minAABB{0.0f};
    glm::vec3 maxAABB{0.0f};

    GLuint aabbVao = 0;
    GLuint aabbVbo = 0;

    Mesh() = default;
    Mesh(std::vector<Vertex>&, glm::vec4);


    void update(glm::mat4);

    void draw(GLint, GLint, glm::vec4);
    void drawNormals(GLint, GLint);
    void drawVertices(GLint, GLint);
    void drawBoundingBox(GLint, GLint);

    void destroy();
    
    std::string toString();
private:
    GLuint createVBO(std::vector<Vertex>&, GLenum = GL_STATIC_DRAW);
    GLuint createVAO();

    void buildNormalBuffers(float length = 0.2f);

    void calculateAABB();
    void buildAABBBuffer();
};
