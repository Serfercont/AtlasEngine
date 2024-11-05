#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "MyWindow.h"
#include "Mesh.h"

class ModuleImporter {
public:
    ModuleImporter();
    ~ModuleImporter();

    bool loadFBX(const std::string& filePath);
    GLuint loadTexture(const char*);

    GLuint getTextureID() { return textureID; }

    GLuint getTextureID() const;

    void setWindow(MyWindow* window);
    const std::vector<Mesh>& getMeshes() const;  // Declaración del método getMeshes
    std::vector<Mesh> meshes;


private:
    std::vector<float> vertices;
    std::vector<float> UvCoords;
    std::vector<unsigned int> indices;
    GLuint textureID = 0;
   

    MyWindow* _window = nullptr;
};