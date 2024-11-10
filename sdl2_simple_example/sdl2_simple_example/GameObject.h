#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>
#include "Texture.h"
#include "Mesh.h"
#include "Transform.h"

class GameObject {
public:
    GameObject(Texture* texture = nullptr, const std::string& name = "");

    void addMesh(Mesh* mesh);
    const std::vector<Mesh*>& getMeshes() const;

    void setTexture(Texture* newTexture);
    Texture* getTexture() const;

    void setTransform(const Transform& newTransform);
    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::vec3 getScale() const;
    const Transform& getTransform() const { return transform; }

    void draw() const;

    const std::string& getName() const;
    void setName(const std::string& newName);

    GameObject* getParent() const;
    const std::vector<GameObject*>& getChildren() const;

    void addChild(GameObject* child);


private:
    std::string name;
    Texture* texture;
    Transform transform;
    GameObject* parent;
    std::vector<GameObject*> children;
    std::vector<Mesh*> meshes;
};

#endif // GAMEOBJECT_H
