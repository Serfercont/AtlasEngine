#include "ModuleScene.h"
#include <GL/glew.h>

ModuleScene::ModuleScene() {}

ModuleScene::~ModuleScene() {
    // Liberar memoria de los GameObjects y sus texturas
    for (GameObject* obj : gameObjects) {
        delete obj;
    }
    gameObjects.clear();
}

void ModuleScene::loadModelData(const std::vector<float>& vertices, const std::vector<float>& uvs, const std::vector<unsigned int>& indices, const std::string& name, const Transform& transform) {
    // Crear un nuevo Mesh
    Mesh* mesh = new Mesh(vertices, uvs, indices);

    // Crear un nuevo GameObject con el Mesh y sin textura inicial
    GameObject* gameObject = new GameObject(mesh, nullptr);
    gameObject->setTransform(transform);

    // Almacenar el GameObject en el vector de la escena
    gameObjects.push_back(gameObject);
    gameObjectNames.push_back(name);
}

void ModuleScene::setTexture(GLuint textureID) {
    // Crear una nueva textura con el ID proporcionado
    Texture* newTexture = new Texture(textureID);

    // Asignar la textura a todos los GameObjects existentes
    for (auto& obj : gameObjects) {
        obj->setTexture(newTexture);
    }
}

void ModuleScene::setCheckerTexture(GLuint checkerTextureID) {
    // Usar el mismo método de setTexture para asignar la textura a todos los GameObjects
    setTexture(checkerTextureID);
}

void ModuleScene::drawScene() {
    // Limpiar el buffer de color y profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Dibujar cada GameObject en la escena
    for (const auto& obj : gameObjects) {
        glPushMatrix();  // Guardar la matriz actual

        obj->draw();     // Llamar al método draw de cada GameObject

        glPopMatrix();   // Restaurar la matriz original
    }
}

void ModuleScene::setMeshes(const std::vector<Mesh>& newMeshes) {
    // Limpiar las mallas anteriores
    meshes.clear();
    gameObjects.clear();  // Limpiar GameObjects anteriores también

    // Crear un GameObject para cada nuevo Mesh en el vector de mallas
    for (const auto& mesh : newMeshes) {
        Mesh* newMesh = new Mesh(mesh.vertices, mesh.uvCoords, mesh.indices);
        GameObject* gameObject = new GameObject(newMesh, nullptr); // Inicialmente sin textura

        gameObjects.push_back(gameObject);
    }
}

void ModuleScene::renderMeshes() {
    for (GameObject* gameObject : gameObjects) {
        if (Mesh* mesh = gameObject->getMesh()) {
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
    }
}

void ModuleScene::addGameObject(GameObject* gameObject) {
    gameObjects.push_back(gameObject);
}
