#pragma once
#include <string>
#include <vector>
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"

class GameObject {
public:
    GameObject(Mesh* mesh, Texture* texture, const std::string& name = "");  
    void draw() const;
    void setTexture(Texture* texture);
    Texture* getTexture() const;
    Mesh* getMesh() const;
    void setTransform(const Transform& newTransform);

    const std::string& getName() const;
    void setName(const std::string& newName); 

    GameObject* getParent() const;
    const std::vector<GameObject*>& getChildren() const;
    void addChild(GameObject* child);

    Mesh* mesh;
    Texture* texture;
    Transform transform;

private:
    std::string name;
    GameObject* parent;
    std::vector<GameObject*> children;
};
