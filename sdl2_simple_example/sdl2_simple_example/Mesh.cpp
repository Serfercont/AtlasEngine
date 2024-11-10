#include "Mesh.h"
#include <array>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
Mesh::Mesh(const std::vector<float>& vertices, const std::vector<float>& uvs, const std::vector<GLuint>& indices)
    : vertices(vertices), uvCoords(uvs), indices(indices) {
    setupMesh();
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboUVs);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
    glBufferData(GL_ARRAY_BUFFER, uvCoords.size() * sizeof(float), uvCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::render() const {
    glBindVertexArray(vao);

    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glVertexPointer(3, GL_FLOAT, 0, nullptr);

    if (!uvCoords.empty()) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
        glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (!uvCoords.empty()) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glBindVertexArray(0);
}

int Mesh::getVertexCount() const {
    return vertices.size() / 3;
}

int Mesh::getFaceCount() const {
    return indices.size() / 3;
}

int Mesh::getTexCoordCount() const {
    return uvCoords.size() / 2;
}

bool Mesh::hasNormals() const {
   
    return false; 
}
