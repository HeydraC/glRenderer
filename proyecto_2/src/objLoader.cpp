#define TINYOBJLOADER_IMPLEMENTATION
#include "objLoader.h"

bool loadObject(std::string filename, std::vector<Mesh> &outMeshes){
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    std::filesystem::path modelsPath = std::filesystem::path(SRC) / "models";
    std::filesystem::path fullPath = modelsPath / filename;


    bool ret = tinyobj::LoadObj(
        &attrib,
        &shapes,
        &materials,
        &err,
        fullPath.c_str(),
        (modelsPath.string() + "/").c_str(),
        true
    );
    
    if (!err.empty()) {
        std::cout<<"Error al cargar "<<filename<<": "<<err<<std::endl;
        return false;
    }
    if (!ret) {
        return false;
    }

    //Definir mínimo y máximo
    glm::vec3 minBound(std::numeric_limits<float>::max());
    glm::vec3 maxBound(std::numeric_limits<float>::lowest());

    for (int s = 0; s < shapes.size(); s++) {
        std::vector<Vertex> meshVertices;
        int index_offset = 0;

        glm::vec4 diffuseColor{1.0f, 1.0f, 1.0f, 1.0f};
        bool colorAssigned = false;

        //Iterando por las caras del submallado
        for (int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            if (!colorAssigned) {
                int mat_id = shapes[s].mesh.material_ids[f];
                if (mat_id >= 0 && mat_id < materials.size()) {
                    diffuseColor = glm::vec4(
                        materials[mat_id].diffuse[0],
                        materials[mat_id].diffuse[1],
                        materials[mat_id].diffuse[2],
                        1.0f
                    );
                    colorAssigned = true;
                }
            }

            std::vector<Vertex> faceVertices;
            bool missingNormals = false;

            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                
                Vertex vertex;

                vertex.pos.x = attrib.vertices[3 * idx.vertex_index + 0];
                vertex.pos.y = attrib.vertices[3 * idx.vertex_index + 1];
                vertex.pos.z = attrib.vertices[3 * idx.vertex_index + 2];

                minBound = glm::min(minBound, vertex.pos);
                maxBound = glm::max(maxBound, vertex.pos);

                if (idx.normal_index >= 0) {
                    vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
                    vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
                    vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];

                    float len = glm::length(vertex.normal);
                    vertex.normal = (len > 0.00001f) ? (vertex.normal / len) : glm::vec3(0.0f, 1.0f, 0.0f);
                }else{
                    missingNormals = true;
                }

                faceVertices.push_back(vertex);
            }

            if (missingNormals && fv >= 3) {
                glm::vec3 edge1 = faceVertices[1].pos - faceVertices[0].pos;
                glm::vec3 edge2 = faceVertices[2].pos - faceVertices[0].pos;
                glm::vec3 newNormal = glm::cross(edge1, edge2);

                float len = glm::length(newNormal);
                newNormal = (len > 0.00001f) ? (newNormal / len) : glm::vec3(0.0f, 1.0f, 0.0f);

                for (size_t v = 0; v < fv; v++) {
                    if (shapes[s].mesh.indices[index_offset + v].normal_index < 0) {
                        faceVertices[v].normal = newNormal;
                    }
                }
            }

            meshVertices.insert(meshVertices.end(), faceVertices.begin(), faceVertices.end());
            index_offset += fv;
        }

        outMeshes.emplace_back(meshVertices, diffuseColor);
    }

    glm::vec3 center = (minBound + maxBound) * 0.5f;
    glm::vec3 extents = maxBound - minBound;
    float maxDim = std::max(extents.x, std::max(extents.y, extents.z));
    float scale = (maxDim > 0.00001f) ? (2.0f / maxDim) : 1.0f;

    for (Mesh& mesh : outMeshes) {
        for (Vertex& vert : mesh.vertices) {
            vert.pos = (vert.pos - center) * scale;
        }
    }

    return true;
}