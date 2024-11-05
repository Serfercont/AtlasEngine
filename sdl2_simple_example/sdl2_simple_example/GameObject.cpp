#include "GameObject.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

// Constructor
GameObject::GameObject(Mesh* mesh, Texture* texture)
    : mesh(mesh), texture(texture), transform() {} // Inicializa la transformación por defecto

// Destructor
GameObject::~GameObject() {
    delete mesh;          // Libera la malla
    delete texture;       // Libera la textura si existe
}

// Establece una nueva textura
void GameObject::setTexture(Texture* newTexture) {
    if (texture) delete texture; // Libera la textura anterior
    texture = newTexture;
}

// Establece la transformación del objeto
void GameObject::setTransform(const Transform& newTransform) {
    transform = newTransform; // Asigna la nueva transformación
}

// Dibuja el objeto en la escena
void GameObject::draw() const {
    glPushMatrix(); // Guardar la matriz actual

    // Aplica las transformaciones
    glTranslatef(transform.position.x, transform.position.y, transform.position.z);
    glRotatef(transform.rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(transform.rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(transform.rotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(transform.scale.x, transform.scale.y, transform.scale.z);

    if (texture) {
        texture->bind(); // Vincula la textura si está disponible
    }
    if (mesh) {
        mesh->render(); // Renderiza la malla
    }
    if (texture) {
        texture->unbind(); // Desvincula la textura
    }

    glPopMatrix(); // Restaurar la matriz anterior
}
