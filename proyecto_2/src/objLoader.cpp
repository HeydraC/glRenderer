#include "objLoader.h"

bool loadObject(std::string filename, std::vector<Vertex> &outVertices){
        tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    std::istringstream iss(filename);

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, &iss);

    if (!err.empty()) {
        std::cout<<"Error al cargar "<<filename<<err<<std::endl;
        exit(1);
    }
    if (!ret) {
        return false;
    }

    for (int s = 0; s < shapes.size(); s++) {
        int index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                Vertex vertex;

                vertex.pos.x = attrib.vertices[3 * idx.vertex_index + 0];
                vertex.pos.y = attrib.vertices[3 * idx.vertex_index + 1];
                vertex.pos.z = attrib.vertices[3 * idx.vertex_index + 2];

                if (idx.normal_index >= 0) {
                    vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
                    vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
                    vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];
                }else{
                    
                }

                outVertices.push_back(vertex);
            }
            index_offset += fv;
        }
    }
    return true;
}