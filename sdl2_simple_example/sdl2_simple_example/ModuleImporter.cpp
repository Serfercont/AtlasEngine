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
#include "GameObject.h"
#include <codecvt>
#include "stb_image.h"
#include <assimp/cimport.h>
#include "Mesh.h"
#include "ModuleScene.h"
#include <SDL2/SDL_events.h>


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
        fprintf(stderr, "Error al cargar el archivo: %s\n", importer.GetErrorString());
        return false;
    }
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
    if (boolChekerTexture) {
		checkerTexture = createCheckerTexture();
        textureID = checkerTexture;
    } else {
        textureID = loadTexture(textureFile);
        if (textureID == 0) {
            std::cerr << "Error al cargar la textura desde el archivo: " << textureFile << std::endl;
            textureID = checkerTexture;  
        }
    }
    Texture* texture = new Texture(textureID);
    
    for (unsigned int i = 0; i < ai_scene->mNumMeshes; i++) {
        const aiMesh* aimesh = ai_scene->mMeshes[i];
        printf("\nMalla %u:\n", i);
        printf(" N�mero de v�rtices: %u\n", aimesh->mNumVertices);

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
        GameObject* gameObject = new GameObject(mesh, texture);  

    
        scene->addGameObject(gameObject);

        meshes.push_back(*mesh);
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
        textureID = textID;
        return textureID;
}

GLuint ModuleImporter::createCheckerTexture() {
    const int width = 64;
    const int height = 64;
    GLubyte checkerImage[height][width][4];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
            checkerImage[i][j][0] = (GLubyte)c;
            checkerImage[i][j][1] = (GLubyte)c;
            checkerImage[i][j][2] = (GLubyte)c;
            checkerImage[i][j][3] = (GLubyte)255;
        }
    }

    GLuint checkerTexture;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &checkerTexture);
    glBindTexture(GL_TEXTURE_2D, checkerTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // U
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // V
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

    return checkerTexture;
}


GLuint ModuleImporter::getTextureID() const {
    return textureID;
}

const std::vector<Mesh>& ModuleImporter::getMeshes() const {
    return meshes;
}