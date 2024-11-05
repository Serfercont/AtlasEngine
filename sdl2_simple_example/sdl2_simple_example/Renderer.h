#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Importer.h"
#include "Camera.h"

class Renderer {
public:
    void render(const std::vector<Mesh>& meshes, GLuint textureID, const Camera& camera);
};

#endif
