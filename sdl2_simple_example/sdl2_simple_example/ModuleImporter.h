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
    GLuint loadTexture(const char*);

    GLuint createCheckerTexture();

    GLuint getTextureID() { return textureID; }

    GLuint getTextureID() const;

    void setWindow(MyWindow* window);
    const std::vector<Mesh>& getMeshes() const;
    //void toggleTexture();

    std::vector<Mesh> meshes;
    bool boolChekerTexture = true;


private:
    std::vector<float> vertices;
    std::vector<float> UvCoords;
    std::vector<unsigned int> indices;
    GLuint textureID = 0;
	GLuint checkerTexture;
	
   

    MyWindow* _window = nullptr;
};