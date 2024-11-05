#include "ModuleScene.h"
#include <GL/glew.h>

ModuleScene::ModuleScene() {}

ModuleScene::~ModuleScene() {
    for (GameObject* obj : gameObjects) {
        delete obj;
    }
    gameObjects.clear();
}

void ModuleScene::loadModelData(const std::vector<float>& vertices, const std::vector<float>& uvs, const std::vector<unsigned int>& indices, const std::string& name, const Transform& transform) {
    Mesh* mesh = new Mesh(vertices, uvs, indices);
    GameObject* gameObject = new GameObject(mesh, nullptr); // Inicializa con textura nula

    gameObject->setTransform(transform); // Establece la transformación del objeto

    gameObjects.push_back(gameObject); // Agrega el objeto a la lista
    gameObjectNames.push_back(name); // Guarda el nombre del objeto
}

// Ejemplo para cargar texturas después de agregar varios GameObjects
void ModuleScene::setTexture(GLuint textureID) {
    Texture* newTexture = new Texture(textureID); // Crea una nueva textura

    for (auto& obj : gameObjects) {
        obj->setTexture(newTexture); // Asigna la textura a cada objeto
    }
}

void ModuleScene::setCheckerTexture(GLuint checkerTextureID) {
    setTexture(checkerTextureID);
}

void ModuleScene::drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& obj : gameObjects) {
        glPushMatrix();  // Guardar la matriz actual

        obj->draw();  // Dibujar el objeto

        glPopMatrix();   // Restaurar la matriz previa
    }
}

void ModuleScene::setMeshes(const std::vector<Mesh>& newMeshes) {
    meshes = newMeshes;
}

void ModuleScene::renderMeshes() const {
    for (const auto& mesh : meshes) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, mesh.uvCoords.data());

        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data());

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}