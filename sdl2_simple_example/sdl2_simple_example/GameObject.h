#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"

class GameObject {
public:
    GameObject(Mesh* mesh, Texture* texture);

    void draw() const;
    void setTexture(Texture* texture);

    Texture* getTexture() const;
    Mesh* getMesh() const;

    void setTransform(const Transform& newTransform);

    Mesh* mesh;
    Texture* texture;
    Transform transform;
};