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

    bool loadFBX(const std::string& filePath, ModuleScene* scene, const char* textureFile);
    GLuint loadTexture(const char* path);

    GLuint getTextureID() const;
    void setWindow(MyWindow* window);

    const std::vector<Mesh>& getLoadedMeshes() const;
    const std::vector<Mesh>& getMeshes() const;

private:
    std::vector<Mesh> meshes;
    std::vector<Mesh> loadedMeshes;
    GLuint textureID = 0;
    MyWindow* _window = nullptr;
};

