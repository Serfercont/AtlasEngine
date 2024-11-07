#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "MyWindow.h"
#include "ModuleScene.h"
#include "Mesh.h"

class ModuleImporter {
public:
    ModuleImporter();
    ~ModuleImporter();

    bool loadFBX(const std::string& filePath, ModuleScene* scene, const char* textureFile);
    GLuint loadTexture(const char*);

    GLuint getTextureID() { return textureID; }

    GLuint getTextureID() const;

    void setWindow(MyWindow* window);
    const std::vector<Mesh>& getMeshes() const; 
    std::vector<Mesh> meshes;


private:
    std::vector<float> vertices;
    std::vector<float> UvCoords;
    std::vector<unsigned int> indices;
    GLuint textureID = 0;
   

    MyWindow* _window = nullptr;
};