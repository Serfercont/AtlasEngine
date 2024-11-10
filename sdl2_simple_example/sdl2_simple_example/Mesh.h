#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Mesh {
public:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> uvCoords;
    std::vector<GLuint> indices;
    GLuint vao, vboVertices, vboUVs, ebo;

    Mesh(const std::vector<float>& vertices, const std::vector<float>& uvs, const std::vector<unsigned int>& indices);

    void setupMesh();
    void render() const;

    int getVertexCount() const;

    int getFaceCount() const;

    int getTexCoordCount() const;

    bool hasNormals() const;

  
    const std::vector<float>& getVertices() const { return vertices; }
    const std::vector<float>& getUVs() const { return uvCoords; }
    const std::vector<GLuint>& getIndices() const { return indices; }

};