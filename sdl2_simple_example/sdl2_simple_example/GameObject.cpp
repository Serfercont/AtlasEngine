#include "GameObject.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>


GameObject::GameObject(Mesh* mesh, Texture* texture)
    : mesh(mesh), texture(texture), transform() {} 

GameObject::~GameObject() {
    delete mesh;          
    delete texture;       
}

void GameObject::setTexture(Texture* newTexture) {
    if (texture) delete texture;
    texture = newTexture;
}


void GameObject::setTransform(const Transform& newTransform) {
    transform = newTransform; 
}

Texture* GameObject::getTexture() const {
    return texture;
}

Mesh* GameObject::getMesh() const {
    return mesh;
}


void GameObject::draw() const {
    glPushMatrix();

    glTranslatef(transform.position.x, transform.position.y, transform.position.z);
    glRotatef(transform.rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(transform.rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(transform.rotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(transform.scale.x, transform.scale.y, transform.scale.z);

    if (texture) {
        texture->bind(); 
    }
    if (mesh) {
        mesh->render();
    }
    if (texture) {
        texture->unbind(); 
    }

    glPopMatrix(); 
}
