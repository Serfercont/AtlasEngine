#include "ModuleScene.h"
#include <GL/glew.h>

ModuleScene::ModuleScene() {}

void ModuleScene::loadModelData(const std::vector<float>& vertices, const std::vector<float>& uvs, const std::vector<unsigned int>& indices, const std::string& name, const Transform& transform) {
    Mesh* mesh = new Mesh(vertices, uvs, indices);

    GameObject* gameObject = new GameObject(mesh, nullptr);
    gameObject->setTransform(transform);

    gameObjects.push_back(gameObject);
    gameObjectNames.push_back(name);
}
// Nueva implementación de setTexture que recibe un GameObject específico
void ModuleScene::setTexture(GameObject* gameObject, GLuint textureID) {
    Texture* newTexture = new Texture(textureID);
    gameObject->setTexture(newTexture);
}

// Función original de setTexture (dejar intacta si se usa para texturas globales)
void ModuleScene::setTexture(GLuint textureID) {
    Texture* newTexture = new Texture(textureID);
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
        glPushMatrix();

        if (Texture* texture = obj->getTexture()) {
            glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
        }

        obj->draw(); // Dibuja cada GameObject con su textura individual

        glBindTexture(GL_TEXTURE_2D, 0);  // Desvincular textura para evitar interferencias
        glPopMatrix();
    }
}

void ModuleScene::setMeshes(const std::vector<Mesh>& newMeshes) {
    // No limpiamos `gameObjects` ni `meshes`

    for (const auto& mesh : newMeshes) {
        Mesh* newMesh = new Mesh(mesh.vertices, mesh.uvCoords, mesh.indices);
        GameObject* gameObject = new GameObject(newMesh, nullptr);

        // Agrega a gameObjects sin borrar los existentes
        gameObjects.push_back(gameObject);
        meshes.push_back(*newMesh);
    }
}

void ModuleScene::renderMeshes() {
    for (GameObject* gameObject : gameObjects) {
        if (Mesh* mesh = gameObject->getMesh()) {
            if (Texture* texture = gameObject->getTexture()) {
                glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
            }

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

            glBindTexture(GL_TEXTURE_2D, 0); 

        }
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
    gameObjectNames.clear();
    meshes.clear();
}



void ModuleScene::addGameObject(GameObject* gameObject) {
    gameObjects.push_back(gameObject);
    // No sobrescribimos la textura de los objetos existentes
}