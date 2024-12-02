#define GLM_ENABLE_EXPERIMENTAL
#include "ModuleScene.h"
#include <GL/glew.h>
#include <iostream>
#include <imgui_impl_opengl3.h>
#include <glm/gtx/string_cast.hpp>


ModuleScene::ModuleScene() {}

void ModuleScene::loadModelData(const std::vector<float>& vertices, const std::vector<float>& uvs, const std::vector<unsigned int>& indices, const std::string& name, const Transform& transform) {
    Mesh* mesh = new Mesh(vertices, uvs, indices);
    GameObject* gameObject = new GameObject(nullptr, name);

    gameObject->addMesh(mesh);  
    gameObject->setTransform(transform);    
    gameObjects.push_back(gameObject);
    std::cout << "GameObject creado con nombre: " << name << std::endl;
}


void ModuleScene::setTexture(GLuint textureID) {
    Texture* newTexture = new Texture(textureID, "Default Texture Path");
    for (auto& obj : gameObjects) { 
        obj->setTexture(newTexture);
    }
}

void ModuleScene::setCheckerTexture(GLuint checkerTextureID) {
    setTexture(checkerTextureID);
}

void ModuleScene::drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& obj : gameObjects) {
        if (obj == selectedGameObject) {
                
            std::cout << "Drawing Selected GameObject: " << obj->getName() << std::endl;
            std::cout << "Position: " << glm::to_string(obj->getTransform().position) << std::endl;
            std::cout << "Rotation: " << glm::to_string(obj->getTransform().rotation) << std::endl;
            std::cout << "Scale: " << glm::to_string(obj->getTransform().scale) << std::endl;
        }
        obj->Update();
        obj->draw();
    }
}


void ModuleScene::setMeshes(const std::vector<Mesh>& newMeshes) {
    gameObjects.clear(); 

    GameObject* gameObject = new GameObject(nullptr, "GameObject");

    for (const auto& mesh : newMeshes) {
        Mesh* newMesh = new Mesh(mesh.vertices, mesh.uvCoords, mesh.indices);
        gameObject->addMesh(newMesh);  
    }

    gameObjects.push_back(gameObject); 
}


void ModuleScene::renderMeshes() {
    for (GameObject* gameObject : gameObjects) {
        if (Texture* texture = gameObject->getTexture()) {
            glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
        }
        for (Mesh* mesh : gameObject->getMeshes()) {
            glBegin(GL_TRIANGLES);
            for (size_t i = 0; i < mesh->indices.size(); i += 3) {
                unsigned int index1 = mesh->indices[i];
                unsigned int index2 = mesh->indices[i + 1];
                unsigned int index3 = mesh->indices[i + 2];

                glTexCoord2f(mesh->uvCoords[index1 * 2], mesh->uvCoords[index1 * 2 + 1]);
                glVertex3f(mesh->vertices[index1 * 3], mesh->vertices[index1 * 3 + 1], mesh->vertices[index1 * 3 + 2]);

                glTexCoord2f(mesh->uvCoords[index2 * 2], mesh->uvCoords[index2 * 2 + 1]);
                glVertex3f(mesh->vertices[index2 * 3], mesh->vertices[index2 * 3 + 1], mesh->vertices[index2 * 3 + 2]);

                glTexCoord2f(mesh->uvCoords[index3 * 2], mesh->uvCoords[index3 * 2 + 1]);
                glVertex3f(mesh->vertices[index3 * 3], mesh->vertices[index3 * 3 + 1], mesh->vertices[index3 * 3 + 2]);
            }
            glEnd();
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}


void ModuleScene::clearGameObjects() {
    for (GameObject*& obj : gameObjects) {
        if (obj) { 
            delete obj;
            obj = nullptr;  
        }
    }
    gameObjects.clear();
    meshes.clear();
}

void ModuleScene::addGameObject(GameObject* gameObject) {
    gameObjects.push_back(gameObject);
}


void getGameObjectNamesRecursive(GameObject* gameObject, std::vector<std::string>& names) {
    names.push_back(gameObject->getName());
    for (auto& child : gameObject->getChildren()) {
        getGameObjectNamesRecursive(child, names);
    }
}
    
std::vector<std::string> ModuleScene::getGameObjectNames() const {
    std::vector<std::string> names;
    for (const auto& gameObject : gameObjects) {
        getGameObjectNamesRecursive(gameObject, names);
    }
    return names;
}
void ModuleScene::selectGameObject(GameObject* go) {
    if (go) {
        selectedGameObject = go;
        std::cout << "ModuleScene: Selected GameObject: " << go->getName() << std::endl;
        std::cout << "Position: "
            << go->getTransform().position.x << ", "
            << go->getTransform().position.y << ", "
            << go->getTransform().position.z << std::endl;
    }
    else {
        std::cout << "ModuleScene: Attempting to select null GameObject!" << std::endl;
        selectedGameObject = nullptr;
    }
}
GameObject* ModuleScene::getSelectedGameObject() const {
    return selectedGameObject;  
}
std::vector<GameObject*> ModuleScene::getGameObjects() {
    return gameObjects;
}
