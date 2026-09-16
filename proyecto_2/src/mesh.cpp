#include "mesh.h"

Mesh::Mesh(const float* vertices, size_t sizeInBytes, int count){
    vertexCount = count;
    vbo = createVBO(vertices, sizeInBytes, GL_DYNAMIC_DRAW);
    vao = createVAO();
}

GLuint Mesh::createVBO(const float* vertices, size_t size, GLenum usage){
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vbo;
}

GLuint Mesh::createVAO(){
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vao;
}

void Mesh::update(const float* newVertices, size_t sizeInBytes, int count){
    vertexCount = count;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeInBytes, newVertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw(){
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

void Mesh::destroy(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}