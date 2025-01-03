#include "GameObject.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject(Texture* texture, const std::string& name)
    : texture(texture), transform(), name(name), parent(nullptr) {}

void GameObject::addMesh(Mesh* mesh) {
    meshes.push_back(mesh);
}

const std::vector<Mesh*>& GameObject::getMeshes() const {
    return meshes;
}

void GameObject::setTexture(Texture* newTexture) {
    if (texture) delete texture;
    texture = newTexture;
}

Texture* GameObject::getTexture() const {
    return texture;
}

void GameObject::setTransform(const Transform& newTransform) {
    transform = newTransform;
}
glm::vec3 GameObject::getPosition() const {
    return transform.position;  
}

glm::vec3 GameObject::getRotation() const {
    return transform.rotation;  
}

glm::vec3 GameObject::getScale() const {
    return transform.scale;  
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

    for (Mesh* mesh : meshes) {
        if (mesh) {
            mesh->render();
        }
    }

    if (texture) {
        texture->unbind();
    }

    glPopMatrix();

    for (GameObject* child : children) {
        if (child) {
            child->draw();
        }
    }
}

const std::string& GameObject::getName() const {
    return name;
}

void GameObject::setName(const std::string& newName) {
    name = newName;
}

GameObject* GameObject::getParent() const {
    return parent;
}

const std::vector<GameObject*>& GameObject::getChildren() const {
    return children;
}

void GameObject::addChild(GameObject* child) {
    if (child) {
        child->parent = this;
        children.push_back(child);
    }
}
