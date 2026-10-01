#include "mesh.h"

Mesh::Mesh(std::string fileName){
    if (!loadObject(fileName, vertices))
        return;
    
    vbo = createVBO(vertices, GL_DYNAMIC_DRAW);
    vao = createVAO();
    objectColor = {0.0f, 0.0f, 1.0f, 1.0f};
}

GLuint Mesh::createVBO(std::vector<Vertex>& vertices, GLenum usage){
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vbo;
}

GLuint Mesh::createVAO(){
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vao;
}

void Mesh::update(const float* newVertices){
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 0, newVertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw(GLuint shaderProgram, GLint modelLocation, GLint objectColorLocation){
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model[0][0]);
    glUniform4fv(objectColorLocation, 1, &objectColor[0]);
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
    glBindVertexArray(0);
}

void Mesh::destroy(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}