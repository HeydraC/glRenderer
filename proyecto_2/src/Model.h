#pragma once

#include "mesh.h"
#include "objLoader.h"

struct Model{
    std::vector<Mesh> meshes;

    glm::vec4 objectColor{1.0f};

    glm::mat4 model{1.0f};

    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};

    bool wireframe = false;
    bool showNormals = false;
    bool showVertices = false;
    bool showBoundingBox = false;

    Model() = default;

    void updateMeshes();

    void updateColor();
 
    bool load(std::string);

    void makeSphere();

    void makePyramid();

    void makeCube();

    void draw(GLint, GLint);

    void destroy();

    std::string toString();
};