#include "Model.h"
#include "mesh.h"

void Model::updateMeshes(){
    model = glm::mat4{1.0f};

    model = glm::translate(model, position);

    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, scale);

    for (Mesh& mesh : meshes){
        mesh.update(model);
    }
}

void Model::updateColor(){
    for (Mesh& mesh : meshes){
        mesh.objectColor *= objectColor;
    }
}

bool Model::load(std::string filename){
    return loadObject(filename, meshes);
}

void addQuad(std::vector<Vertex>& vertices, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal){
    vertices.push_back({p0, normal});
    vertices.push_back({p1, normal});
    vertices.push_back({p2, normal});

    vertices.push_back({p0, normal});
    vertices.push_back({p2, normal});
    vertices.push_back({p3, normal});
}

void Model::makeCube(){
    std::vector<Vertex> vertices;

    addQuad(vertices, {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f});
    addQuad(vertices, { 0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f});
    addQuad(vertices, {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f});
    addQuad(vertices, { 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f});
    addQuad(vertices, {-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f});
    addQuad(vertices, {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f});

    meshes.emplace_back(vertices, glm::vec4{1.0f});
}

void addTriangle(std::vector<Vertex>& vertices, glm::vec3 p0, glm::vec3 p1,glm::vec3 p2){
    glm::vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
    vertices.push_back({p0, normal});
    vertices.push_back({p1, normal});
    vertices.push_back({p2, normal});
}

void Model::makePyramid(){
    std::vector<Vertex> vertices;

    glm::vec3 apex(0.0f, 0.5f, 0.0f);
    glm::vec3 b0(-0.5f, -0.5f,  0.5f); 
    glm::vec3 b1( 0.5f, -0.5f,  0.5f);
    glm::vec3 b2( 0.5f, -0.5f, -0.5f);
    glm::vec3 b3(-0.5f, -0.5f, -0.5f);

    addTriangle(vertices, b0, b1, apex); 
    addTriangle(vertices, b1, b2, apex); 
    addTriangle(vertices, b2, b3, apex); 
    addTriangle(vertices, b3, b0, apex); 

    glm::vec3 downNormal(0.0f, -1.0f, 0.0f);
    vertices.push_back({b0, downNormal});
    vertices.push_back({b2, downNormal});
    vertices.push_back({b1, downNormal});

    vertices.push_back({b0, downNormal});
    vertices.push_back({b3, downNormal});
    vertices.push_back({b2, downNormal});

    meshes.emplace_back(vertices, glm::vec4{1.0f});
}

void Model::makeSphere() {
    std::vector<Vertex> vertices;

    float t = (1.0f + std::sqrt(5.0f)) / 2.0f; //phi

    std::vector<glm::vec3> basePositions = {
        glm::normalize(glm::vec3(-1.0f,  t,  0.0f)) * 0.5f,
        glm::normalize(glm::vec3( 1.0f,  t,  0.0f)) * 0.5f,
        glm::normalize(glm::vec3(-1.0f, -t,  0.0f)) * 0.5f,
        glm::normalize(glm::vec3( 1.0f, -t,  0.0f)) * 0.5f,

        glm::normalize(glm::vec3( 0.0f, -1.0f,  t)) * 0.5f,
        glm::normalize(glm::vec3( 0.0f,  1.0f,  t)) * 0.5f,
        glm::normalize(glm::vec3( 0.0f, -1.0f, -t)) * 0.5f,
        glm::normalize(glm::vec3( 0.0f,  1.0f, -t)) * 0.5f,

        glm::normalize(glm::vec3(  t,  0.0f, -1.0f)) * 0.5f,
        glm::normalize(glm::vec3(  t,  0.0f,  1.0f)) * 0.5f,
        glm::normalize(glm::vec3( -t,  0.0f, -1.0f)) * 0.5f,
        glm::normalize(glm::vec3( -t,  0.0f,  1.0f)) * 0.5f
    };

    int indices[20][3] = {
        {0, 11, 5}, {0, 5, 1},  {0, 1, 7},   {0, 7, 10}, {0, 10, 11},
        {1, 5, 9},  {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
        {3, 9, 4},  {3, 4, 2},  {3, 2, 6},   {3, 6, 8},  {3, 8, 9},
        {4, 9, 5},  {2, 4, 11}, {6, 2, 10},  {8, 6, 7},  {9, 8, 1}
    };

    struct Triangle {
        glm::vec3 a, b, c;
    };

    std::vector<Triangle> triangles;

    for (int i = 0; i < 20; ++i) {
        triangles.push_back({
            basePositions[indices[i][0]],
            basePositions[indices[i][1]],
            basePositions[indices[i][2]]
        });
    }

    for (int s = 0; s < 2; ++s) {
        std::vector<Triangle> next;

        for (Triangle& tri : triangles) {
            glm::vec3 m0 = glm::normalize(tri.a + tri.b) * 0.5f;
            glm::vec3 m1 = glm::normalize(tri.b + tri.c) * 0.5f;
            glm::vec3 m2 = glm::normalize(tri.c + tri.a) * 0.5f;

            next.push_back({tri.a, m0, m2});
            next.push_back({tri.b, m1, m0});
            next.push_back({tri.c, m2, m1});
            next.push_back({m0, m1, m2});
        }
        triangles = std::move(next);
    }

    for (Triangle& tri : triangles) {
        vertices.push_back({tri.a, glm::normalize(tri.a)});
        vertices.push_back({tri.b, glm::normalize(tri.b)});
        vertices.push_back({tri.c, glm::normalize(tri.c)});
    }

    meshes.emplace_back(vertices, glm::vec4{1.0f});
}

void Model::draw(GLint modelLocation, GLint objectColorLocation){
    if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (Mesh& mesh : meshes){
        mesh.draw(modelLocation, objectColorLocation, objectColor);

        if (showNormals || mesh.showNormals) mesh.drawNormals(modelLocation, objectColorLocation);

        if (showVertices || mesh.showVertices) mesh.drawVertices(modelLocation, objectColorLocation);

        if (showBoundingBox || mesh.showBoundingBox) mesh.drawBoundingBox(modelLocation, objectColorLocation);
    }    

    if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Model::destroy(){
    for (Mesh& mesh : meshes)
        mesh.destroy();
}

std::string Model::toString(){
    std::string buffer;

    for (Mesh& mesh : meshes)
        buffer += mesh.toString() + '\n';

    buffer += "m ";

    buffer += std::to_string(objectColor.r) + ' '
            + std::to_string(objectColor.g) + ' '
            + std::to_string(objectColor.b) + ' '
            + std::to_string(objectColor.a);

    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            buffer += ' ' + std::to_string(model[i][j]);
        }
    }

    buffer += '\n';
    
    return buffer;
}