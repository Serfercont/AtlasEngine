#pragma once
#include <vector>
#include <string>
#include "GameObject.h"

class ModuleScene {
public:
    GameObject* getLastCreatedGameObject() const {
        if (!gameObjects.empty()) {
            return gameObjects.back();  // Retorna el último GameObject agregado.
        }
        return nullptr;  // Retorna nullptr si no hay GameObjects.
    }

    ModuleScene();
    void loadModelData(const std::vector<float>& vertices, const std::vector<float>& uvs, const std::vector<unsigned int>& indices, const std::string& name, const Transform& transform = Transform());
    void setTexture(GLuint textureID);
    void setCheckerTexture(GLuint checkerTextureID);
    void drawScene();
    void setMeshes(const std::vector<Mesh>& meshes);
    void renderMeshes();

    void clearGameObjects();

    void addGameObject(GameObject* gameObject);

    std::vector<GameObject*> gameObjects;
    std::vector<std::string> gameObjectNames;

    int selectedGameObjectIndex = -1;
private:
    std::vector<Mesh> meshes;
};