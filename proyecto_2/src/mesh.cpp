#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> &_vertices, glm::vec4 _objectColor){
    vertices = _vertices;
    objectColor = _objectColor;
    
    vbo = createVBO(vertices, GL_DYNAMIC_DRAW);
    vao = createVAO();

    buildNormalBuffers();
    calculateAABB();
    buildAABBBuffer();
}

GLuint Mesh::createVBO(std::vector<Vertex>& vertices, GLenum usage){
    model = glm::mat4(1.0f);

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

void Mesh::calculateAABB() {
    if (vertices.empty()) return;

    minAABB = vertices[0].pos;
    maxAABB = vertices[0].pos;

    for (const Vertex& v : vertices) {
        minAABB = glm::min(minAABB, v.pos);
        maxAABB = glm::max(maxAABB, v.pos);
    }
}

void Mesh::buildAABBBuffer() {
    if (aabbVao != 0) {
        glDeleteVertexArrays(1, &aabbVao);
        glDeleteBuffers(1, &aabbVbo);
        aabbVao = 0;
        aabbVbo = 0;
    }

    // 8 local corners of the box
    glm::vec3 c[8] = {
        {minAABB.x, minAABB.y, minAABB.z}, // 0
        {maxAABB.x, minAABB.y, minAABB.z}, // 1
        {maxAABB.x, maxAABB.y, minAABB.z}, // 2
        {minAABB.x, maxAABB.y, minAABB.z}, // 3
        {minAABB.x, minAABB.y, maxAABB.z}, // 4
        {maxAABB.x, minAABB.y, maxAABB.z}, // 5
        {maxAABB.x, maxAABB.y, maxAABB.z}, // 6
        {minAABB.x, maxAABB.y, maxAABB.z}  // 7
    };

    // 12 edges (24 vertices)
    int indices[] = {
        0, 1,  1, 2,  2, 3,  3, 0, // bottom face (z min)
        4, 5,  5, 6,  6, 7,  7, 4, // top face (z max)
        0, 4,  1, 5,  2, 6,  3, 7  // vertical connecting edges
    };

    std::vector<Vertex> boxLines;
    boxLines.reserve(24);

    for (int idx : indices) {
        Vertex v;
        v.pos = c[idx];
        v.normal = glm::vec3(0.0f, 1.0f, 0.0f); // Dummy normal for base.vert
        boxLines.push_back(v);
    }

    glGenVertexArrays(1, &aabbVao);
    glGenBuffers(1, &aabbVbo);

    glBindVertexArray(aabbVao);
    glBindBuffer(GL_ARRAY_BUFFER, aabbVbo);
    glBufferData(GL_ARRAY_BUFFER, boxLines.size() * sizeof(Vertex), boxLines.data(), GL_STATIC_DRAW);

    // Layout matches base.vert layout (0: pos, 1: normal)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::buildNormalBuffers(float length) {
    std::vector<Vertex> normalLines;
    normalLines.reserve(vertices.size() * 2);

    for (const Vertex& v : vertices) {
        Vertex start;
        start.pos = v.pos;
        start.normal = v.normal;
        normalLines.push_back(start);

        Vertex end;
        end.pos = v.pos + glm::normalize(v.normal) * length;
        end.normal = v.normal;
        normalLines.push_back(end);
    }

    normalVertexCount = normalLines.size();

    glGenVertexArrays(1, &normalVao);
    glGenBuffers(1, &normalVbo);

    glBindVertexArray(normalVao);
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(GL_ARRAY_BUFFER, normalLines.size() * sizeof(Vertex), normalLines.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::update(glm::mat4 mat){
    mat = glm::translate(mat, position);

    mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    mat = glm::scale(mat, scale);

    model = mat;
}

void Mesh::draw(GLint modelLocation, GLint objectColorLocation, glm::vec4 modelColor){
    if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::vec4 oColor = objectColor * modelColor;

    bool isTransparent = oColor.a < 1.0f;

    if (isTransparent) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE); 
    }

    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model[0][0]);
    glUniform4fv(objectColorLocation, 1, &oColor[0]);
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
    glBindVertexArray(0);

    if (isTransparent) {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Mesh::drawNormals(GLint modelLoc, GLint colorLoc) {
    if (normalVao == 0 || normalVertexCount == 0) return;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

    glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

    glBindVertexArray(normalVao);
    glDrawArrays(GL_LINES, 0, normalVertexCount);
    glBindVertexArray(0);
}

void Mesh::drawVertices(GLint modelLoc, GLint colorLoc){
    glPointSize(6.0f);
    glDepthFunc(GL_LEQUAL);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glUniform4f(colorLoc, 1.0f, 0.2f, 0.2f, 1.0f);

    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, vertices.size());
    glBindVertexArray(0);

    glPointSize(1.0f);
    glDepthFunc(GL_LESS);
}

void Mesh::drawBoundingBox(GLint modelLoc, GLint colorLoc) {
    if (aabbVao == 0 && !vertices.empty()) {
        calculateAABB();
        buildAABBBuffer();
    }

    if (aabbVao == 0) return;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

    glBindVertexArray(aabbVao);
    glDrawArrays(GL_LINES, 0, 24);
    glBindVertexArray(0);
}

void Mesh::destroy(){
    if (vao != 0) glDeleteVertexArrays(1, &vao);
    if (vbo != 0) glDeleteBuffers(1, &vbo);
    if (normalVao != 0) glDeleteVertexArrays(1, &normalVao);
    if (normalVbo != 0) glDeleteBuffers(1, &normalVbo);
    vao = 0;
    vbo = 0;
    normalVao = 0;
    normalVbo = 0;
}

std::string Mesh::toString(){
    std::string buffer;

    buffer += std::to_string(objectColor.r) + ' '
            + std::to_string(objectColor.g) + ' '
            + std::to_string(objectColor.b) + ' '
            + std::to_string(objectColor.a) + ' ';
         
    for (Vertex vertice : vertices){
        buffer += std::to_string(vertice.pos.x) + ' '
                + std::to_string(vertice.pos.y) + ' '
                + std::to_string(vertice.pos.z) + ' ';

        buffer += std::to_string(vertice.normal.x) + ' '
                + std::to_string(vertice.normal.y) + ' '
                + std::to_string(vertice.normal.z) + ' ';
    }

    buffer += 'M';

    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            buffer += ' ' + std::to_string(model[i][j]);
        }
    }

    return buffer;
}