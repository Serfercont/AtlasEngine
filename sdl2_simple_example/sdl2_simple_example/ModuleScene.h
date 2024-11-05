#pragma once
#include <vector>
#include <string>
#include "GameObject.h"

class ModuleScene {
public:
    ModuleScene();
    ~ModuleScene();
    void loadModelData(const std::vector<float>& vertices, const std::vector<float>& uvs, const std::vector<unsigned int>& indices, const std::string& name, const Transform& transform = Transform());
    void setTexture(GLuint textureID);
    void setCheckerTexture(GLuint checkerTextureID);
    void drawScene();
    void setMeshes(const std::vector<Mesh>& meshes);  // Configura las mallas a renderizar
    void renderMeshes() const;  // Renderiza las mallas

    std::vector<GameObject*> gameObjects;
    std::vector<std::string> gameObjectNames;

    // Índice del objeto seleccionado (-1 si no hay ninguno)
    int selectedGameObjectIndex = -1;
private:
    std::vector<Mesh> meshes;
};