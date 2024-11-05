#include "ModuleImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <iostream>
#include <fstream>
#include "MyWindow.h"
#include <codecvt>
#include "stb_image.h"
#include <assimp/cimport.h>
#include "Mesh.h"
#include <SDL2/SDL_events.h>

using vec3 = glm::dvec3;

float modelScale = 1.0f; // Global scaling factor for the model
vec3 modelCenter(0.0f); // Model center for positioning

ModuleImporter::ModuleImporter() {
    ilInit();
    iluInit();
    ilutRenderer(ILUT_OPENGL);
}

ModuleImporter::~ModuleImporter() {
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }
}

void ModuleImporter::setWindow(MyWindow* window) {
    _window = window;
}

void centerModel(const aiScene* scene) {
    if (scene->mNumMeshes == 0) return; // Comprobar si hay meshes
    aiVector3D min, max;
    min = max = scene->mMeshes[0]->mVertices[0];

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        for (unsigned int v = 0; v < scene->mMeshes[i]->mNumVertices; v++) {
            aiVector3D vertex = scene->mMeshes[i]->mVertices[v];
            min.x = min(min.x, vertex.x);
            min.y = min(min.y, vertex.y);
            min.z = min(min.z, vertex.z);
            max.x = max(max.x, vertex.x);
            max.y = max(max.y, vertex.y);
            max.z = max(max.z, vertex.z);
        }
    }
    modelCenter = vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
    modelScale = 2.0f / glm::length(vec3(max.x - min.x, max.y - min.y, max.z - min.z));
    //modelScale = 2.0f / glm::length(vec3(max.x - min.x, max.y - min.y, max.z - min.z));
}

// Dentro de la clase Importer, agrega un método para crear una textura checker
GLuint ModuleImporter::createCheckerTexture() {
    const int width = 64;
    const int height = 64;
    std::vector<GLubyte> checkerImage(width * height * 4);

    // Generar el patrón checker
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int c = ((x / 8) % 2) == ((y / 8) % 2) ? 255 : 0;
            checkerImage[(y * width + x) * 4 + 0] = (GLubyte)c;
            checkerImage[(y * width + x) * 4 + 1] = (GLubyte)c;
            checkerImage[(y * width + x) * 4 + 2] = (GLubyte)c;
            checkerImage[(y * width + x) * 4 + 3] = 255;  // Alpha
        }
    }

    GLuint checkerTexture;
    glGenTextures(1, &checkerTexture);
    glBindTexture(GL_TEXTURE_2D, checkerTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return checkerTexture;
}

bool ModuleImporter::loadFBX(const std::string& filePath) {
    const aiScene* scene = aiImportFile(filePath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene) {
            fprintf(stderr, "Error en cargar el archivo: %s\n", aiGetErrorString());
            return false;
        }
    
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            const aiMesh* aimesh = scene->mMeshes[i];
            printf("\nMalla %u:\n", i);
            printf(" Numero de v?rtices: %u\n", aimesh->mNumVertices);
    
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
            Mesh mesh(vertices, uvCoords, indices);
            meshes.push_back(mesh);
        }
        return true;
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
        textureID = textID; // Asignar el textureID
        return textureID;
    }

// Método para obtener la textura cargada o checker
GLuint ModuleImporter::getTextureID() const {
    return textureID;
}

const std::vector<Mesh>& ModuleImporter::getMeshes() const {
    return meshes;
}
