#include "ModuleImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <iostream>
#include "MyWindow.h"
#include "GameObject.h"
#include "stb_image.h"
#include "Mesh.h"
#include "ModuleScene.h"

using vec3 = glm::dvec3;

ModuleImporter::ModuleImporter() {
    ilInit();
    iluInit();
    ilutRenderer(ILUT_OPENGL);
}

void ModuleImporter::setWindow(MyWindow* window) {
    _window = window;
}

bool ModuleImporter::loadFBX(const std::string& filePath, ModuleScene* scene, const char* textureFile) {
    Assimp::Importer importer;
    const aiScene* ai_scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!ai_scene) {
        std::cerr << "Error al cargar el archivo: " << importer.GetErrorString() << std::endl;
        return false;
    }

    // Limpiar loadedMeshes antes de cargar nuevas mallas
    loadedMeshes.clear();
    textureID = loadTexture(textureFile); // Cargar la textura

    for (unsigned int i = 0; i < ai_scene->mNumMeshes; i++) {
        const aiMesh* aimesh = ai_scene->mMeshes[i];

        std::vector<float> vertices;
        std::vector<float> uvCoords;
        std::vector<unsigned int> indices;

        for (unsigned int v = 0; v < aimesh->mNumVertices; v++) {
            vertices.push_back(aimesh->mVertices[v].x);
            vertices.push_back(aimesh->mVertices[v].y);
            vertices.push_back(aimesh->mVertices[v].z);

            if (aimesh->HasTextureCoords(0)) {
                uvCoords.push_back(aimesh->mTextureCoords[0][v].x);
                uvCoords.push_back(aimesh->mTextureCoords[0][v].y);
            }
            else {
                uvCoords.push_back(0.0f);
                uvCoords.push_back(0.0f);
            }
        }

        for (unsigned int f = 0; f < aimesh->mNumFaces; f++) {
            const aiFace& face = aimesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        Mesh* mesh = new Mesh(vertices, uvCoords, indices);
        GameObject* gameObject = new GameObject(mesh, nullptr);

        // Agrega el GameObject a la escena
        scene->addGameObject(gameObject);

        // Añadir a meshes y loadedMeshes
        meshes.push_back(*mesh);
        loadedMeshes.push_back(*mesh);  // Solo las mallas de la carga actual
    }
    return true;
}

const std::vector<Mesh>& ModuleImporter::getLoadedMeshes() const {
    return loadedMeshes;
}

GLuint ModuleImporter::loadTexture(const char* path) {
    int width, height, channels;
    unsigned char* imageData = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

    if (imageData == nullptr) {
        std::cerr << "Error: No se pudo cargar la textura " << path << std::endl;
        return 0;
    }

    GLuint textID;
    glGenTextures(1, &textID);
    glBindTexture(GL_TEXTURE_2D, textID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(imageData);
    textureID = textID; // Asignar la textura cargada a textureID
    return textureID;
}

GLuint ModuleImporter::getTextureID() const {
    return textureID;
}

const std::vector<Mesh>& ModuleImporter::getMeshes() const {
    return meshes;
}
