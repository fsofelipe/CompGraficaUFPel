#include "../include/mesh.hpp"
Mesh::Mesh(){}

Mesh::Mesh(char *path, int ID) {
    bool res = loadOBJ(path, vertices, uvs, normals);
    
    indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
    
    this->ID = ID;
}

std::vector<glm::vec3> Mesh::getVertices() {
    return indexed_vertices;
}

std::vector<glm::vec2> Mesh::getUvs() {
    return indexed_uvs;
}

std::vector<glm::vec3> Mesh::getNormals() {
    return indexed_normals;
}

std::vector<unsigned short> Mesh::getIndices() {
    return indices;
}

int Mesh::getID() {
    return ID;
}
