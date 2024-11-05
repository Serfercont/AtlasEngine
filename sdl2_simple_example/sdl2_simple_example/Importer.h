#ifndef IMPORTER_H
#define IMPORTER_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

struct Mesh {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<GLfloat> uvCoords;
};

class Importer {
public:
    GLuint loadTexture(const char* path);
    void loadFBX(const std::string& filePath);
    const std::vector<Mesh>& getMeshes() const;

private:
    std::vector<Mesh> meshes;
};

#endif
